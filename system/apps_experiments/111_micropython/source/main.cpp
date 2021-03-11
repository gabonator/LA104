#include <library.h>

#include "../../os_host/source/framework/BufferedIo.h"
uint8_t gFatSharedBuffer[BIOS::FAT::SharedBufferSize];
CBufferedReader mReader;

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

extern "C" void mp_reader_new_file(mp_reader_t *reader, const char *filename)
{
    _ASSERT(0);
}

enum { EXEC_FLAG_SOURCE_IS_RAW_CODE = 8 };
extern "C" int parse_compile_execute(const void *source, mp_parse_input_kind_t input_kind, int exec_flags);

#ifdef _ARM
__attribute__((__section__(".entry")))
#endif
int _main() {
    _ASSERT(sizeof(gFatSharedBuffer) >= BIOS::SYS::GetAttribute(BIOS::SYS::EAttribute::DiskSectorSize));
    BIOS::FAT::SetSharedBuffer(gFatSharedBuffer);

    int stack_dummy;
    stack_top = (char *)&stack_dummy;

    #if MICROPY_ENABLE_GC
    gc_init(heap, heap + sizeof(heap));
    #endif

    mp_init();
    
    if (mReader.Open((char*)"test.pyc"))
    {
        BIOS::DBG::Print("[run]");
        
        _ASSERT(mReader.GetFileSize() < BufferSectorSize);
        mp_raw_code_t* p = mp_raw_code_load_mem(mReader.GetBuffer(), mReader.GetFileSize());
        parse_compile_execute(p, MP_PARSE_FILE_INPUT, EXEC_FLAG_SOURCE_IS_RAW_CODE);

        BIOS::DBG::Print("[done]");
    } else {
        BIOS::DBG::Print("[test.pyc not found]");
    }
    BIOS::SYS::DelayMs(1000);
    mp_deinit();

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
