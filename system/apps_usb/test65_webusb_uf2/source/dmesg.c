/*
The MIT License (MIT)

Copyright (c) 2017 Lancaster University.

Permission is hereby granted, free of charge, to any person obtaining a
copy of this software and associated documentation files (the "Software"),
to deal in the Software without restriction, including without limitation
the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.
*/

#include "dmesg.h"
#include <string.h>
#include "libopencm3/cm3/cortex.h"

CodalLogStore codalLogStore;

static void logwrite(const char *msg);

static void logwriten(const char *msg, int l)
{
    if (codalLogStore.ptr + l >= sizeof(codalLogStore.buffer))
    {
        const int jump = sizeof(codalLogStore.buffer) / 4;
        codalLogStore.ptr -= jump;
        memmove(codalLogStore.buffer, codalLogStore.buffer + jump, codalLogStore.ptr);
        // zero-out the rest so it looks OK in the debugger
        memset(codalLogStore.buffer + codalLogStore.ptr, 0, sizeof(codalLogStore.buffer) - codalLogStore.ptr);
    }
    if (l + codalLogStore.ptr >= sizeof(codalLogStore.buffer))
    {
        logwrite("DMESG line too long!\n");
        return;
    }
    memcpy(codalLogStore.buffer + codalLogStore.ptr, msg, l);
    codalLogStore.ptr += l;
    codalLogStore.buffer[codalLogStore.ptr] = 0;
}

static void logwrite(const char *msg)
{
    logwriten(msg, strlen(msg));
}

static void writeDecNum(char *buf, int32_t n)
{
    if (n < 0) {
        *buf++ = '-';
        n = -n;
    }

    if (n == 0) {
        *buf++ = '0';
        *buf++ = 0;
        return;
    }

    char tmp[20];
    int i = 0;
    while (n > 0) {
        tmp[i++] = (n % 10) + '0';
        n /= 10;
    }

    while (--i > 0) {
        *buf++ = tmp[i];
    }

    *buf = 0;
}

static void writeNum(char *buf, uint32_t n, bool full)
{
    int i = 0;
    int sh = 28;
    while (sh >= 0)
    {
        int d = (n >> sh) & 0xf;
        if (full || d || sh == 0 || i)
        {
            buf[i++] = d > 9 ? 'A' + d - 10 : '0' + d;
        }
        sh -= 4;
    }
    buf[i] = 0;
}

static void logwritenum(uint32_t n, bool full, bool hex)
{
    char buff[20];

    if (hex)
    {
        writeNum(buff, n, full);
        logwrite("0x");
    }
    else
    {
        writeDecNum(buff, n);
    }

    logwrite(buff);
}

void codal_dmesg(const char *format, ...)
{
    va_list arg;
    va_start(arg, format);
    codal_vdmesg(format, arg);
    va_end(arg);
}

void codal_vdmesg(const char *format, va_list ap)
{
    const char *end = format;

    CM_ATOMIC_BLOCK() {

        while (*end)
        {
            if (*end++ == '%')
            {
                logwriten(format, end - format - 1);
                uint32_t val = va_arg(ap, uint32_t);
                switch (*end++)
                {
                case 'c':
                    logwriten((const char *)&val, 1);
                    break;
                case 'd':
                    logwritenum(val, false, false);
                    break;
                case 'x':
                    logwritenum(val, false, true);
                    break;
                case 'p':
                case 'X':
                    logwritenum(val, true, true);
                    break;
                case 's':
                    logwrite((char *)(void *)val);
                    break;
                case '%':
                    logwrite("%");
                    break;
                default:
                    logwrite("???");
                    break;
                }
                format = end;
            }
        }
        logwriten(format, end - format);
        logwrite("\n");

    }
}
