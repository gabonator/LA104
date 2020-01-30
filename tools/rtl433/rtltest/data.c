/*
 * A general structure for extracting hierarchical data from the devices;
 * typically key-value pairs, but allows for more rich data as well.
 *
 * Copyright (C) 2015 by Erkki Seppälä <flux@modeemi.fi>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdarg.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>
//#include <stdlib.h>
#include "alloc.h"

#include <stdbool.h>
#include "limits.h"
// gethostname() needs _XOPEN_SOURCE 500 on unistd.h
//#define _XOPEN_SOURCE 500

#ifndef _MSC_VER
#include <unistd.h>
#endif

#ifdef _WIN32
  #if !defined(_WIN32_WINNT) || (_WIN32_WINNT < 0x0600)
  #undef _WIN32_WINNT
  #define _WIN32_WINNT 0x0600   /* Needed to pull in 'struct sockaddr_storage' */
  #endif

  #include <winsock2.h>
  #include <ws2tcpip.h>
#else
  #include <netdb.h>
  #include <netinet/in.h>

  #define SOCKET          int
  #define INVALID_SOCKET  -1
#endif

#include <time.h>

//#include "term_ctl.h"
//#include "abuf.h"

#include "data.h"

#ifdef _WIN32
  #define _POSIX_HOST_NAME_MAX  128
  #undef  close   /* We only work with sockets here */
  #define close(s)              closesocket (s)
  #define perror(str)           ws2_perror (str)

  static void ws2_perror (const char *str)
  {
    if (str && *str)
       fprintf (stderr, "%s: ", str);
    fprintf (stderr, "Winsock error %d.\n", WSAGetLastError());
  }
#endif

typedef void* (*array_elementwise_import_fn)(void*);
typedef void (*array_element_release_fn)(void*);
typedef void (*value_release_fn)(void*);

typedef struct {
    /* what is the element size when put inside an array? */
    int array_element_size;

    /* is the element boxed (ie. behind a pointer) when inside an array?
       if it's not boxed ("unboxed"), json dumping function needs to make
       a copy of the value beforehand, because the dumping function only
       deals with boxed values.
     */
    bool array_is_boxed;

    /* function for importing arrays. strings are specially handled (as they
       are copied deeply), whereas other arrays are just copied shallowly
       (but copied nevertheless) */
    array_elementwise_import_fn array_elementwise_import;

    /* a function for releasing an element when put in an array; integers
     * don't need to be released, while ie. strings and arrays do. */
    array_element_release_fn array_element_release;

    /* a function for releasing a value. everything needs to be released. */
    value_release_fn value_release;
} data_meta_type_t;

static data_meta_type_t dmt[DATA_COUNT] = {
    //  DATA_DATA
    { .array_element_size       = sizeof(data_t*),
      .array_is_boxed           = true,
      .array_elementwise_import = NULL,
      .array_element_release    = (array_element_release_fn) data_free,
      .value_release            = (value_release_fn) data_free },

    //  DATA_INT
    { .array_element_size       = sizeof(int),
      .array_is_boxed           = false,
      .array_elementwise_import = NULL,
      .array_element_release    = NULL,
      .value_release            = (value_release_fn) free },

    //  DATA_DOUBLE
    { .array_element_size       = sizeof(double),
      .array_is_boxed           = false,
      .array_elementwise_import = NULL,
      .array_element_release    = NULL,
      .value_release            = (value_release_fn) free },

    //  DATA_STRING
    { .array_element_size       = sizeof(char*),
      .array_is_boxed           = true,
      .array_elementwise_import = (array_elementwise_import_fn) strdup,
      .array_element_release    = (array_element_release_fn) free,
      .value_release            = (value_release_fn) free },

    //  DATA_ARRAY
    { .array_element_size       = sizeof(data_array_t*),
      .array_is_boxed           = true,
      .array_elementwise_import = NULL,
      .array_element_release    = (array_element_release_fn) data_array_free ,
      .value_release            = (value_release_fn) data_array_free },
};

static bool import_values(void *dst, void *src, int num_values, data_type_t type)
{
    int element_size = dmt[type].array_element_size;
    array_elementwise_import_fn import = dmt[type].array_elementwise_import;
    if (import) {
        for (int i = 0; i < num_values; ++i) {
            void *copy = import(*(void **)((char *)src + element_size * i));
            if (!copy) {
                --i;
                while (i >= 0) {
                    free(*(void **)((char *)dst + element_size * i));
                    --i;
                }
                return false;
            } else {
                *((char **)dst + i) = copy;
            }
        }
    } else {
        memcpy(dst, src, element_size * num_values);
    }
    return true; // error is returned early
}

/* data */

data_array_t *data_array(int num_values, data_type_t type, void *values)
{
    data_array_t *array = calloc(1, sizeof(data_array_t));
    if (array) {
        int element_size = dmt[type].array_element_size;
        array->values = calloc(num_values, element_size);
        if (!array->values)
            goto alloc_error;
        if (!import_values(array->values, values, num_values, type))
            goto alloc_error;

        array->num_values = num_values;
        array->type = type;
    }
    return array;

alloc_error:
    if (array)
        free(array->values);
    free(array);
    return NULL;
}

static data_t *vdata_make(data_t *first, const char *key, const char *pretty_key, va_list ap)
{
    data_type_t type;
    data_t *prev = first;
    while (prev && prev->next)
        prev = prev->next;
    char *format = false;
    type = va_arg(ap, data_type_t);
    do {
        data_t *current;
        void *value = NULL;

        switch (type) {
        case DATA_FORMAT:
            format = strdup(va_arg(ap, char *));
            if (!format)
                goto alloc_error;
            type = va_arg(ap, data_type_t);
            continue;
            break;
        case DATA_COUNT:
            assert(0);
            break;
        case DATA_DATA:
            value = va_arg(ap, data_t *);
            break;
        case DATA_INT:
            value = malloc(sizeof(int));
            if (value)
                *(int *)value = va_arg(ap, int);
            break;
        case DATA_DOUBLE:
            value = malloc(sizeof(double));
            if (value)
                *(double *)value = va_arg(ap, double);
            break;
        case DATA_STRING:
            value = strdup(va_arg(ap, char *));
            break;
        case DATA_ARRAY:
            value = va_arg(ap, data_t *);
            break;
        }

        // also some null arguments are mapped to an alloc error;
        // that's ok, because they originate (typically..) from
        // an alloc error anyway
        if (!value)
            goto alloc_error;

        current = calloc(1, sizeof(*current));
        if (!current)
            goto alloc_error;
        if (prev)
            prev->next = current;

        current->key = strdup(key);
        if (!current->key)
            goto alloc_error;
        current->pretty_key = strdup(pretty_key ? pretty_key : key);
        if (!current->pretty_key)
            goto alloc_error;
        current->type = type;
        current->format = format;
        current->value = value;
        current->next = NULL;

        prev = current;
        if (!first)
            first = current;

        key = va_arg(ap, const char *);
        if (key) {
            pretty_key = va_arg(ap, const char *);
            type = va_arg(ap, data_type_t);
            format = NULL;
        }
    } while (key);
    va_end(ap);

    return first;

alloc_error:
    data_free(first);
    return NULL;
}

data_t *data_make(const char *key, const char *pretty_key, ...)
{
    va_list ap;
    va_start(ap, pretty_key);
    data_t *result = vdata_make(NULL, key, pretty_key, ap);
    va_end(ap);
    return result;
}

data_t *data_append(data_t *first, const char *key, const char *pretty_key, ...)
{
    va_list ap;
    va_start(ap, pretty_key);
    data_t *result = vdata_make(first, key, pretty_key, ap);
    va_end(ap);
    return result;
}

data_t *data_prepend(data_t *first, const char *key, const char *pretty_key, ...)
{
    va_list ap;
    va_start(ap, pretty_key);
    data_t *result = vdata_make(NULL, key, pretty_key, ap);
    va_end(ap);

    if (!result)
        return first;

    data_t *prev = result;
    while (prev && prev->next)
        prev = prev->next;
    prev->next = first;

    return result;
}

void data_array_free(data_array_t *array)
{
    array_element_release_fn release = dmt[array->type].array_element_release;
    if (release) {
        int element_size = dmt[array->type].array_element_size;
        for (int i = 0; i < array->num_values; ++i)
            release(*(void **)((char *)array->values + element_size * i));
    }
    free(array->values);
    free(array);
}

data_t *data_retain(data_t *data)
{
    if (data)
        ++data->retain;
    return data;
}

void data_free(data_t *data)
{
    if (data && data->retain) {
        --data->retain;
        return;
    }
    while (data) {
        data_t *prev_data = data;
        if (dmt[data->type].value_release)
            dmt[data->type].value_release(data->value);
        free(data->format);
        free(data->pretty_key);
        free(data->key);
        data = data->next;
        free(prev_data);
    }
}

/* data output */

void data_output_print(data_output_t *output, data_t *data)
{
    if (!output)
        return;
    output->print_data(output, data, NULL);
}

void data_output_start(struct data_output *output, const char **fields, int num_fields)
{
    if (!output || !output->output_start)
        return;
    output->output_start(output, fields, num_fields);
}

void data_output_poll(struct data_output *output)
{
    if (!output || !output->output_poll)
        return;
    output->output_poll(output);
}

void data_output_free(data_output_t *output)
{
    if (!output)
        return;
    output->output_free(output);
}

/* output helpers */

void print_value(data_output_t *output, data_type_t type, void *value, char *format)
{
    switch (type) {
    case DATA_FORMAT:
    case DATA_COUNT:
        assert(0);
        break;
    case DATA_DATA:
        output->print_data(output, value, format);
        break;
    case DATA_INT:
        output->print_int(output, *(int *)value, format);
        break;
    case DATA_DOUBLE:
        output->print_double(output, *(double *)value, format);
        break;
    case DATA_STRING:
        output->print_string(output, value, format);
        break;
    case DATA_ARRAY:
        output->print_array(output, value, format);
        break;
    }
}

void print_array_value(data_output_t *output, data_array_t *array, char *format, int idx)
{
    int element_size = dmt[array->type].array_element_size;
#ifdef RTL_433_NO_VLAs
    char *buffer = alloca (element_size);
#else
    char buffer[element_size];
#endif

    if (!dmt[array->type].array_is_boxed) {
        memcpy(buffer, (void **)((char *)array->values + element_size * idx), element_size);
        print_value(output, array->type, buffer, format);
    } else {
        print_value(output, array->type, *(void **)((char *)array->values + element_size * idx), format);
    }
}

