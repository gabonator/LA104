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
const char* translation = nullptr;
int dummy[1];
compressed_string_t* dummyTranslation = (compressed_string_t*)&dummy;
char* decompress(const compressed_string_t* compressed, char* decompressed) {
    _ASSERT(translation && compressed == dummyTranslation);
    strcpy(decompressed, translation);
    return decompressed;
}

uint16_t decompress_length(const compressed_string_t* compressed) {
    _ASSERT(translation && compressed == dummyTranslation);
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
    BIOS::KEY::GetKey();
#ifdef __APPLE__
    fprintf(stdout, "%s", prompt);
    fgets(line->buf, line->alloc, stdin);
    line->len = strlen(line->buf);
    /*
    std::string l;
    std::cout << prompt;
    std::getline(std::cin, l);
    _ASSERT(l.length() < line->alloc);
    line->len = l.length();
    strcpy(line->buf, l.c_str());
    //fgets(line->buf, line->len, stdin);
     */
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
    mp_pystack_init(stack, stack + (sizeof(stack) / sizeof(size_t)));
    gc_init(heap, heap + sizeof(heap));
    mp_init();
    mp_stack_ctrl_init();

    strcpy(linebuf, R"--(print("uPy")
print("a long string that is not interned")
print("a string that has unicode αβγ chars")
print(b"bytes 1234\x01")
print(123456789)
           
import mini
colors = [0x404080, 0x4060b0, 0x4080d0, 0x40a0ff]
for x in range(0, 320/20):
  for y in range(0, 200/20):
    i = x + y
    c = colors[i % 4]
    mini.color(c)
    mini.bar(x*20, y*20, x*20+18, y*20+18)
)--");
//
//    mini.print(30, 60, "ahoj")

    /*
     i = (x+y)%4
r
     */
    vstr_t line;
    line.alloc = 1023;
    line.len = strlen(linebuf);
    line.buf = linebuf;
    line.fixed_buf = true;
    

    #define EXEC_FLAG_ALLOW_DEBUGGING (2)
    #define EXEC_FLAG_SOURCE_IS_VSTR (16)

    mp_parse_input_kind_t parse_input_kind = MP_PARSE_FILE_INPUT;  //MP_PARSE_SINGLE_INPUT;

    int ret = parse_compile_execute(&line, parse_input_kind, EXEC_FLAG_ALLOW_DEBUGGING /*| EXEC_FLAG_IS_REPL*/ | EXEC_FLAG_SOURCE_IS_VSTR, NULL);
    
    //pyexec_friendly_repl();

    BIOS::SYS::DelayMs(5000);
    mp_deinit();

    return 0;
}

void gc_collect(void) {
    // WARNING: This gc_collect implementation doesn't try to get root
    // pointers from CPU registers, and thus may function incorrectly.
    BIOS::DBG::Print("[gc_collect]");
    gc_collect_start();
//    #if CIRCUITPY_DISPLAYIO
//    gc_collect_root((void**)heap, sizeof(heap) / sizeof(uint32_t));
    gc_collect_end();
}

extern "C" void _HandleAssertion(const char* file, int line, const char* cond)
{
   BIOS::DBG::Print("Assertion failed");
    while (1);
}


int lcdColor = 0xffff;
int lcdBack = 0x0000;

extern "C" mp_obj_t mini_color_set(mp_obj_t value)
{
    uint32_t color = mp_obj_get_int(value);
    lcdColor = RGB32TO565(color);
    return mp_const_none;
}

extern "C" mp_obj_t mini_back_set(mp_obj_t value)
{
    uint32_t color = mp_obj_get_int(value);
    lcdBack = RGB32TO565(color);
    return mp_const_none;
}

extern "C" mp_obj_t mini_print(size_t n_args, const mp_obj_t *args) {
    (void)n_args;

    mp_int_t x, y;
    if (!mp_obj_get_int_maybe(args[0], &x) ||
        !mp_obj_get_int_maybe(args[1], &y) ||
        !MP_OBJ_IS_STR(args[2]))
    {
        mp_raise_ValueError(translate("wrong args"));
        return mp_const_none;
    }

    size_t len = 0;
    const char* msg = mp_obj_str_get_data(args[2], &len);

    BIOS::LCD::Print(x, y, lcdColor, lcdBack, msg);
    return mp_const_none;
}

extern "C" mp_obj_t mini_bar(size_t n_args, const mp_obj_t *args) {
    (void)n_args;
    mp_int_t x1, y1, x2, y2;
    if (!mp_obj_get_int_maybe(args[0], &x1) ||
        !mp_obj_get_int_maybe(args[1], &y1) ||
        !mp_obj_get_int_maybe(args[2], &x2) ||
        !mp_obj_get_int_maybe(args[3], &y2))
    {
        mp_raise_ValueError(translate("wrong args"));
        return mp_const_none;
    }
    BIOS::LCD::Bar(x1, y1, x2, y2, lcdColor);
    return mp_const_none;
}
