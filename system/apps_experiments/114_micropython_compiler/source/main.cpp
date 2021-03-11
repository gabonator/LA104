#include <library.h>

extern "C" {
#include "py/gc.h"
#include "py/lexer.h"
#include "py/runtime.h"
//#include "lib/utils/pyexec.h"
//
#include "py/compile.h"
#include "py/persistentcode.h"

}

#include "../../os_host/source/framework/BufferedIo.h"
uint8_t gFatSharedBuffer[BIOS::FAT::SharedBufferSize];
    CBufferedWriter mWriter;

//mp_lexer_t *mp_lexer_new_from_file(const char *filename) {
//_ASSERT(0);
//    return 0;
//}

extern "C" mp_lexer_t *mp_lexer_new_from_str_len(qstr src_name, const char *str, size_t len, size_t free_len);

extern "C" void mp_hal_stdout_tx_str(const char *str);
extern "C" mp_raw_code_t *mp_raw_code_load_mem(const byte *buf, size_t len);

static char *stack_top;
#if MICROPY_ENABLE_GC
static char heap[2048*2];
#endif

#ifdef GABO_COMPILER
//#include <iostream>

STATIC void fd_print_strn(void *env, const char *str, size_t len) {
    //std::cout << "\n=========code========\n";
        mWriter << CStream((uint8_t*)str, len);
//        char 
    for (int i=0; i<len; i++)
    {
        BIOS::DBG::Print("0x%02x, ", (uint8_t)str[i]);
    }
    //std::cout << "\n=========code========\n";
}


int compile_and_save(const char *file, const char *output_file, const char *source_file) {
    nlr_buf_t nlr;
    BIOS::DBG::Print("[8]");
    if (nlr_push(&nlr) == 0) {
    BIOS::DBG::Print("[9]");
        mp_lexer_t *lex = mp_lexer_new_from_file(file);
    BIOS::DBG::Print("[0]");

        qstr source_name;
        if (source_file == NULL) {
            source_name = lex->source_name;
        } else {
            source_name = qstr_from_str(source_file);
        }
    BIOS::DBG::Print("[1]");

        #if MICROPY_PY___FILE__
        mp_store_global(MP_QSTR___file__, MP_OBJ_NEW_QSTR(source_name));
        #endif

        mp_parse_tree_t parse_tree = mp_parse(lex, MP_PARSE_FILE_INPUT);
        mp_raw_code_t *rc = mp_compile_to_raw_code(&parse_tree, source_name, false);
    BIOS::DBG::Print("[2]");

        vstr_t vstr;
        vstr_init(&vstr, 16);
        if (output_file == NULL) {
            vstr_add_str(&vstr, file);
            vstr_cut_tail_bytes(&vstr, 2);
            vstr_add_str(&vstr, "mpy");
        } else {
            vstr_add_str(&vstr, output_file);
        }
    BIOS::DBG::Print("[3]");

        //mp_raw_code_save_file(rc, vstr_null_terminated_str(&vstr));
        mp_print_t fd_print = {0, fd_print_strn};
    BIOS::DBG::Print("[4]");
        mp_raw_code_save(rc, &fd_print);
    BIOS::DBG::Print("[5]");

        vstr_clear(&vstr);
    BIOS::DBG::Print("[6]");

        nlr_pop();
        return 0;
    } else {
        // uncaught exception
        _ASSERT(0);
        //mp_obj_print_exception(&mp_stderr_print, (mp_obj_t)nlr.ret_val);
        return 1;
    }
}

    static const char* content = R"--(
print("uPy")
print("a long string that is not interned")
print("a string that has unicode αβγ chars")
print(b"bytes 1234\x01")
print(123456780+9)
for i in range(4):
    print(i)
)--";

extern "C" void mp_reader_new_file(mp_reader_t *reader, const char *filename)
{
              BIOS::DBG::Print("[lexer-1]");
    

    mp_reader_new_mem(reader, (const byte*)content, strlen(content), 0);
              BIOS::DBG::Print("[lexer-2]");
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
    BIOS::DBG::Print("[a]");
    _ASSERT(sizeof(gFatSharedBuffer) >= BIOS::SYS::GetAttribute(BIOS::SYS::EAttribute::DiskSectorSize));
    BIOS::FAT::SetSharedBuffer(gFatSharedBuffer);

    int stack_dummy;
    stack_top = (char *)&stack_dummy;
                         BIOS::DBG::Print("[b]");

    #if MICROPY_ENABLE_GC
    gc_init(heap, heap + sizeof(heap));
    #endif
              BIOS::DBG::Print("[c]");

    mp_init();
                 BIOS::DBG::Print("[d]");

#ifdef GABO_COMPILER

    mWriter.Open((char*)"test.pyc");
    compile_and_save("test", "bla", "out");
    mWriter.Close();
#endif
           BIOS::DBG::Print("[e]");

    
    mp_deinit();
         BIOS::DBG::Print("[f]");
        BIOS::FAT::SetSharedBuffer(nullptr);

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

void _HandleAssertion(const char* file, int line, const char* cond)
{
    BIOS::DBG::Print("Assertion failed");
    while (1);
}


extern "C" {
void _HandleAssertionPy(const char* file, int line, const char* cond)
{
    BIOS::DBG::Print("Assertion failed in py");
    while (1);
}
}

extern "C" void nlr_jump_fail(void *val) {
    BIOS::DBG::Print("NLR JUMP failed");
    while (1);
}
