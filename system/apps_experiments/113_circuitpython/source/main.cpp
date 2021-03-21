#include <library.h>
#ifdef __APPLE__
#include <iostream>
#include <string>
#endif

extern "C" {
#include "py/gc.h"
#include "py/lexer.h"
#include "py/runtime.h"
#include "py/parse.h"
#include "lib/utils/pyexec.h"
#include "supervisor/shared/safe_mode.h"
#include "py/emitglue.h"
#include "py/stackctrl.h"

#include "py/objlist.h"
}

extern "C" {
struct _mp_dummy_t {int f;};
_mp_dummy_t mp_sys_stdin_obj;
_mp_dummy_t mp_sys_stdout_obj;
_mp_dummy_t mp_sys_stderr_obj;

void assert_heap_ok() {}
char* decompress(const compressed_string_t* compressed, char* decompressed) {
    _ASSERT(0);
    return (char*)"???";
}

uint16_t decompress_length(const compressed_string_t* compressed) {
    return 3;
}
const compressed_string_t* translate(const char* c)
{
    BIOS::DBG::Print("error: '%s'", c);
    _ASSERT(0);
    return nullptr;
}
bool mp_hal_is_interrupted(void) { return false; }

int readline(vstr_t *line, const char *prompt)
{
#ifdef __APPLE__
    std::string l;
    std::cout << prompt;
    std::getline(std::cin, l);
    _ASSERT(l.length() < line->alloc);
    line->len = l.length();
    strcpy(line->buf, l.c_str());
    //fgets(line->buf, line->len, stdin);
#else
    BIOS::SYS::DelayMs(1000);
    static int counter = 0;
    sprintf(line->buf, "print(\"ahoj %d\")", counter++);
    line->len = strlen(line->buf);
#endif
    return 0;
}

void reset_into_safe_mode(safe_mode_t reason) {
    _ASSERT(0);
    while (1);
}
void mp_reader_new_file(mp_reader_t *reader, const char *filename)
{
    _ASSERT(0);
}

bool stack_ok(void) { return true; }
void supervisor_run_background_tasks_if_tick(void) {}
}

extern const unsigned char test_py[345];
extern unsigned int test_py_len;
enum { EXEC_FLAG_SOURCE_IS_RAW_CODE = 8 };
extern "C" int parse_compile_execute(const void *source, mp_parse_input_kind_t input_kind, int exec_flags, pyexec_result_t *result);


extern "C" void mp_hal_stdout_tx_str(const char *str);
extern "C" mp_raw_code_t *mp_raw_code_load_mem(const byte *buf, size_t len);

static char *stack_top;
#if MICROPY_ENABLE_GC
static char heap[4096];
#endif
#if MICROPY_ENABLE_PYSTACK
//static char stack[2048*32*8];
static char stack[1024];
#endif
    char linebuf[1024];

#ifdef _ARM
__attribute__((__section__(".entry")))
#endif
int _main() {
BIOS::DBG::Print("start====");
    int stack_dummy;

    mp_stack_set_limit(20000 * (BYTES_PER_WORD / 4));
    mp_stack_set_top(&stack_dummy);
    
    #if MICROPY_ENABLE_PYSTACK
    mp_pystack_init(stack, stack + (sizeof(stack) / sizeof(size_t)));
    #endif

    #if MICROPY_ENABLE_GC
    gc_init(heap, heap + sizeof(heap));
    #endif

    mp_init();

    mp_stack_ctrl_init();


//    BIOS::DBG::Print("[run]");
//    mp_raw_code_t* p = mp_raw_code_load_mem(test_py, test_py_len);
//    parse_compile_execute(p, MP_PARSE_FILE_INPUT, EXEC_FLAG_SOURCE_IS_RAW_CODE, NULL);
//
//    BIOS::DBG::Print("[done]");

//    mp_init();
//    mp_obj_list_init((mp_obj_list_t*)mp_sys_path, 0);
//    mp_obj_list_append(mp_sys_path, MP_OBJ_NEW_QSTR(MP_QSTR_)); // current dir (or base dir of the script)
//    mp_obj_list_append(mp_sys_path, MP_OBJ_NEW_QSTR(MP_QSTR__slash_));
//    // Frozen modules are in their own pseudo-dir, e.g., ".frozen".
//    // Prioritize .frozen over /lib.
//    //mp_obj_list_append(mp_sys_path, MP_OBJ_NEW_QSTR(MP_FROZEN_FAKE_DIR_QSTR));
//    mp_obj_list_append(mp_sys_path, MP_OBJ_NEW_QSTR(MP_QSTR__slash_lib));
//
//    mp_obj_list_init((mp_obj_list_t*)mp_sys_argv, 0);

    
    //strcpy(linebuf, "print('ahoj' + str(-7/3))");
    //strcpy(linebuf, "3+5");
    strcpy(linebuf, R"--(print("uPy")
print("a long string that is not interned")
print("a string that has unicode αβγ chars")
print(b"bytes 1234\x01")
print(123456789)
for i in range(4):
   print(i)
)--");

    vstr_t line;
    line.alloc = 1023;
    line.len = strlen(linebuf);
    line.buf = linebuf;
    line.fixed_buf = true;
BIOS::DBG::Print("E");BIOS::SYS::DelayMs(1000);
    

#define EXEC_FLAG_PRINT_EOF (1)
#define EXEC_FLAG_ALLOW_DEBUGGING (2)
#define EXEC_FLAG_IS_REPL (4)
#define EXEC_FLAG_SOURCE_IS_RAW_CODE (8)
#define EXEC_FLAG_SOURCE_IS_VSTR (16)
#define EXEC_FLAG_SOURCE_IS_FILENAME (32)

    mp_parse_input_kind_t parse_input_kind = MP_PARSE_FILE_INPUT; //MP_PARSE_EVAL_INPUT; //MP_PARSE_SINGLE_INPUT;

    int ret = parse_compile_execute(&line, parse_input_kind, EXEC_FLAG_ALLOW_DEBUGGING /*| EXEC_FLAG_IS_REPL*/ | EXEC_FLAG_SOURCE_IS_VSTR, NULL);

BIOS::DBG::Print("F");BIOS::SYS::DelayMs(1000);
    
    pyexec_friendly_repl();

    BIOS::SYS::DelayMs(1000);
    mp_deinit();

    return 0;
}

#if MICROPY_ENABLE_GC
void gc_collect(void) {
    // WARNING: This gc_collect implementation doesn't try to get root
    // pointers from CPU registers, and thus may function incorrectly.
    BIOS::DBG::Print("[gc_collect]");
//return;
//    void *dummy;
//    gc_collect_start();
//    gc_collect_root(&dummy, ((mp_uint_t)stack_top - (mp_uint_t)&dummy) / sizeof(mp_uint_t));
//    gc_collect_end();
//    gc_dump_info();
}
#endif

extern "C" void _HandleAssertion(const char* file, int line, const char* cond)
{
   BIOS::DBG::Print("Assertion failed");
    while (1);
}

