#include <library.h>

extern "C" {
#include "py/gc.h"
#include "py/lexer.h"
#include "py/runtime.h"
#include "lib/utils/pyexec.h"
//
#include "py/compile.h"
#include "py/persistentcode.h"

}

mp_lexer_t *mp_lexer_new_from_file(const char *filename) {
_ASSERT(0);
    return 0;
}


extern "C" void mp_hal_stdout_tx_str(const char *str);
extern "C" mp_raw_code_t *mp_raw_code_load_mem(const byte *buf, size_t len);

static char *stack_top;
#if MICROPY_ENABLE_GC
static char heap[2048*2];
#endif

#ifdef GABO_COMPILER
#include <iostream>

STATIC void fd_print_strn(void *env, const char *str, size_t len) {
    //std::cout << "\n=========code========\n";
    for (int i=0; i<len; i++)
    {
        char c[32];
        sprintf(c, "0x%02x, ", (uint8_t)str[i]);
        std::cout << c;
    }
    //std::cout << "\n=========code========\n";
}


int compile_and_save(const char *file, const char *output_file, const char *source_file) {
    nlr_buf_t nlr;
    if (nlr_push(&nlr) == 0) {
        mp_lexer_t *lex = mp_lexer_new_from_str_len(file);

        qstr source_name;
        if (source_file == NULL) {
            source_name = lex->source_name;
        } else {
            source_name = qstr_from_str(source_file);
        }

        #if MICROPY_PY___FILE__
        mp_store_global(MP_QSTR___file__, MP_OBJ_NEW_QSTR(source_name));
        #endif

        mp_parse_tree_t parse_tree = mp_parse(lex, MP_PARSE_FILE_INPUT);
        mp_raw_code_t *rc = mp_compile_to_raw_code(&parse_tree, source_name, false);

        vstr_t vstr;
        vstr_init(&vstr, 16);
        if (output_file == NULL) {
            vstr_add_str(&vstr, file);
            vstr_cut_tail_bytes(&vstr, 2);
            vstr_add_str(&vstr, "mpy");
        } else {
            vstr_add_str(&vstr, output_file);
        }
        //mp_raw_code_save_file(rc, vstr_null_terminated_str(&vstr));
        mp_print_t fd_print = {0, fd_print_strn};
        mp_raw_code_save(rc, &fd_print);

        vstr_clear(&vstr);

        nlr_pop();
        return 0;
    } else {
        // uncaught exception
        _ASSERT(0);
        //mp_obj_print_exception(&mp_stderr_print, (mp_obj_t)nlr.ret_val);
        return 1;
    }
}
extern "C" void mp_reader_new_file(mp_reader_t *reader, const char *filename)
{
    const char* content = R"--(
print("uPy")
print("a long string that is not interned")
print("a string that has unicode αβγ chars")
print(b"bytes 1234\x01")
print(123456789)
for i in range(4):
    print(i)
)--";
    
    mp_reader_new_mem(reader, (const byte*)content, strlen(content), 0);
}
#else
extern "C" void mp_reader_new_file(mp_reader_t *reader, const char *filename)
{
    _ASSERT(0);
}
#endif

enum { EXEC_FLAG_SOURCE_IS_RAW_CODE = 8 };
extern "C" int parse_compile_execute(const void *source, mp_parse_input_kind_t input_kind, int exec_flags);

#ifdef _ARM
__attribute__((__section__(".entry")))
#endif
int _main() {
    int stack_dummy;
    stack_top = (char *)&stack_dummy;

    #if MICROPY_ENABLE_GC
    gc_init(heap, heap + sizeof(heap));
    #endif

    mp_init();

#ifdef GABO_COMPILER
    compile_and_save("test", "bla", "out");
#endif
    
    const static uint8_t code[] = {0x4d, 0x05, 0x00, 0x3f, 0x20, 0x82, 0x3c, 0x10, 0x0a, 0x00, 0x07, 0x06, 0x6f, 0x75, 0x74, 0x00, 0x11, 0x00, 0x7b, 0x10, 0x06, 0x75, 0x50, 0x79, 0x34, 0x01, 0x59, 0x11, 0x00, 0x7b, 0x23, 0x00, 0x34, 0x01, 0x59, 0x11, 0x00, 0x7b, 0x23, 0x01, 0x34, 0x01, 0x59, 0x11, 0x00, 0x7b, 0x23, 0x02, 0x34, 0x01, 0x59, 0x11, 0x00, 0x7b, 0x22, 0xba, 0xef, 0x9a, 0x15, 0x34, 0x01, 0x59, 0x80, 0x42, 0x0f, 0x80, 0x57, 0x16, 0x02, 0x69, 0x11, 0x00, 0x7b, 0x11, 0x01, 0x34, 0x01, 0x59, 0x81, 0xe5, 0x57, 0x84, 0xd7, 0x43, 0xeb, 0x7f, 0x59, 0x51, 0x63, 0x03, 0x00, 0x73, 0x22, 0x61, 0x20, 0x6c, 0x6f, 0x6e, 0x67, 0x20, 0x73, 0x74, 0x72, 0x69, 0x6e, 0x67, 0x20, 0x74, 0x68, 0x61, 0x74, 0x20, 0x69, 0x73, 0x20, 0x6e, 0x6f, 0x74, 0x20, 0x69, 0x6e, 0x74, 0x65, 0x72, 0x6e, 0x65, 0x64, 0x73, 0x26, 0x61, 0x20, 0x73, 0x74, 0x72, 0x69, 0x6e, 0x67, 0x20, 0x74, 0x68, 0x61, 0x74, 0x20, 0x68, 0x61, 0x73, 0x20, 0x75, 0x6e, 0x69, 0x63, 0x6f, 0x64, 0x65, 0x20, 0xce, 0xb1, 0xce, 0xb2, 0xce, 0xb3, 0x20, 0x63, 0x68, 0x61, 0x72, 0x73, 0x62, 0x0b, 0x62, 0x79, 0x74, 0x65, 0x73, 0x20, 0x31, 0x32, 0x33, 0x34, 0x01, };

    BIOS::DBG::Print("[run]");
    //pyexec_frozen_module("frozentest.py");
    
    mp_raw_code_t* p = mp_raw_code_load_mem(code, sizeof(code));
    parse_compile_execute(p, MP_PARSE_FILE_INPUT, EXEC_FLAG_SOURCE_IS_RAW_CODE);

    BIOS::DBG::Print("[done]");
    BIOS::SYS::DelayMs(1000);
    mp_deinit();

    return 0;
}

#if MICROPY_ENABLE_GC
void gc_collect(void) {
    // WARNING: This gc_collect implementation doesn't try to get root
    // pointers from CPU registers, and thus may function incorrectly.
    BIOS::DBG::Print("[gc_collect]");
return;
    void *dummy;
    gc_collect_start();
    gc_collect_root(&dummy, ((mp_uint_t)stack_top - (mp_uint_t)&dummy) / sizeof(mp_uint_t));
    gc_collect_end();
    gc_dump_info();
}
#endif

extern "C" void dprint(const char* m)
{
    BIOS::DBG::Print("<<%s>>", m);
}

void _HandleAssertion(const char* file, int line, const char* cond)
{
   BIOS::DBG::Print("Assertion failed in %s [%d]", __FILE__, line);
    while (1);
}

extern "C" void nlr_jump_fail(void *val) {
    BIOS::DBG::Print("NLR JUMP failed");
    while (1);
}
