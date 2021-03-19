#include <library.h>

#include <iostream>
#include <string>

extern "C" {
#include "py/gc.h"
#include "py/lexer.h"
#include "py/runtime.h"
#include "py/parse.h"
#include "lib/utils/pyexec.h"
#include "supervisor/shared/safe_mode.h"
#include "py/emitglue.h"
#include "py/stackctrl.h"
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
    std::string l;
    std::cout << prompt;
    std::getline(std::cin, l);
    _ASSERT(l.length() < line->alloc);
    line->len = l.length();
    strcpy(line->buf, l.c_str());
    //fgets(line->buf, line->len, stdin);
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
static char heap[2048*32];
#endif
#if MICROPY_ENABLE_PYSTACK
static char stack[2048*32];
#endif

int run_repl(void) {
    int exit_code = PYEXEC_FORCED_EXIT;
//    stack_resize();
//    filesystem_flush();
//    supervisor_allocation* heap = allocate_remaining_memory();
//    start_mp(heap);
//    autoreload_suspend();
//    new_status_color(REPL_RUNNING);
//    if (pyexec_mode_kind == PYEXEC_MODE_RAW_REPL) {
//        exit_code = pyexec_raw_repl();
//    } else {
        exit_code = pyexec_friendly_repl();
//    }
//    cleanup_after_vm(heap);
//    autoreload_resume();
    return exit_code;
}

#ifdef _ARM
__attribute__((__section__(".entry")))
#endif
int _main() {
    int stack_dummy;

    mp_stack_set_limit(2000 * (BYTES_PER_WORD / 4));
    mp_stack_set_top(&stack_dummy);
    
    #if MICROPY_ENABLE_PYSTACK
    mp_pystack_init(stack, stack + (sizeof(stack) / sizeof(size_t)));
    #endif

    #if MICROPY_ENABLE_GC
    gc_init(heap, heap + sizeof(heap));
    #endif

    mp_init();

    mp_stack_ctrl_init();

    BIOS::DBG::Print("[run]");
    mp_raw_code_t* p = mp_raw_code_load_mem(test_py, test_py_len);
    parse_compile_execute(p, MP_PARSE_FILE_INPUT, EXEC_FLAG_SOURCE_IS_RAW_CODE, NULL);

    BIOS::DBG::Print("[done]");
    
    run_repl();

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

void _HandleAssertion(const char* file, int line, const char* cond)
{
   BIOS::DBG::Print("Assertion failed");
    while (1);
}

