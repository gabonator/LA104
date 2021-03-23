#include "assert.h"
#include "py/gc.h"
#include "py/lexer.h"
#include "py/mperrno.h"
#include "py/runtime.h"
#include <string.h>
#include "supervisor/shared/safe_mode.h"

mp_import_stat_t mp_import_stat(const char *path) {
    return MP_IMPORT_STAT_NO_EXIST;
}

mp_obj_t mp_builtin_open(size_t n_args, const mp_obj_t *args, mp_map_t *kwargs) {
    return mp_const_none;
}
MP_DEFINE_CONST_FUN_OBJ_KW(mp_builtin_open_obj, 1, mp_builtin_open);

void nlr_jump_fail(void *val) {
    while (1) {
        ;
    }
}

struct _mp_dummy_t {int f;};
struct _mp_dummy_t mp_sys_stdin_obj;
struct _mp_dummy_t mp_sys_stdout_obj;
struct _mp_dummy_t mp_sys_stderr_obj;

void assert_heap_ok() {}
const char* translation = 0;
int dummy[1];
compressed_string_t* dummyTranslation = (compressed_string_t*)&dummy;
char* decompress(const compressed_string_t* compressed, char* decompressed) {
    assert(translation && compressed == dummyTranslation);
    strcpy(decompressed, translation);
    return decompressed;
}

uint16_t decompress_length(const compressed_string_t* compressed) {
    assert(translation && compressed == dummyTranslation);
    return strlen(translation)+1;
}

const compressed_string_t* translate(const char* c)
{
    translation = c;
    return dummyTranslation;
}

bool mp_hal_is_interrupted(void) { return false; }

int readline(vstr_t *line, const char *prompt)
{
    assert(0);
    return 0;
}

void reset_into_safe_mode(safe_mode_t reason) {
    assert(0);
    while (1);
}

void mp_reader_new_file(mp_reader_t *reader, const char *filename)
{
    assert(0);
}

bool stack_ok(void) { return true; }
void supervisor_run_background_tasks_if_tick(void) {}

extern const unsigned char test_py[345];
extern unsigned int test_py_len;
enum { EXEC_FLAG_SOURCE_IS_RAW_CODE = 8 };

void gc_collect(void)
{
    // TODO: not correctly implemented
    gc_collect_start();
    gc_collect_end();
}

extern void _PrintError(const char* msg, size_t len);
extern void _PrintAnswer(const char* msg, size_t len);

bool errorMessage = false;

void mp_hal_stdout_tx_str(const char *str)
{
    if (errorMessage)
        _PrintError(str, strlen(str));
    else
        _PrintAnswer(str, strlen(str));
}

void mp_hal_stdout_tx_strn(const char *str, size_t len)
{
    if (errorMessage)
        _PrintError(str, len);
    else
        _PrintAnswer(str, len);
}

void mp_hal_set_interrupt_char(int c)
{
    errorMessage = c == -1;
}

void mp_hal_stdout_tx_strn(const char *str, size_t len);
void mp_hal_stdout_tx_str(const char *str);

void mp_hal_stdout_tx_strn_cooked(const char *str, size_t len) {
    const char *last = str;
    while (len--) {
        if (*str == '\n') {
            if (str > last) {
                mp_hal_stdout_tx_strn(last, str - last);
            }
            mp_hal_stdout_tx_strn("\r\n", 2);
            ++str;
            last = str;
        } else {
            ++str;
        }
    }
    if (str > last) {
        mp_hal_stdout_tx_strn(last, str - last);
    }
}
