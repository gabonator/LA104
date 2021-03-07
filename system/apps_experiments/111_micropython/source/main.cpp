#include <library.h>
//#include <stdint.h>
//#include <assert.h>
//#include <stdio.h>
//#include <string.h>
/*
#include "py/compile.h"
#include "py/repl.h"
*/
extern "C" {
#include "py/gc.h"
#include "py/lexer.h"
//#include "py/mperrno.h"
#include "py/runtime.h"
#include "lib/utils/pyexec.h"
}

extern "C" void mp_hal_stdout_tx_str(const char *str);


static char *stack_top;
#if MICROPY_ENABLE_GC
static char heap[2048];
#endif

#ifdef _ARM
__attribute__((__section__(".entry")))
#endif

int main(int argc, char* argv[]) {
    int stack_dummy;
    stack_top = (char *)&stack_dummy;

    #if MICROPY_ENABLE_GC
    gc_init(heap, heap + sizeof(heap));
    #endif

    mp_init();

    BIOS::DBG::Print("[run]");
    pyexec_frozen_module("frozentest.py");
    BIOS::DBG::Print("[done]");
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
