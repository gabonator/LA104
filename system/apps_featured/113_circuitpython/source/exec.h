#include <py/mpconfig.h>
#include <genhdr/mpversion.h>
#include <py/gc.h>
#include <py/runtime.h>
#include <py/stackctrl.h>
#include <py/repl.h>
#include <py/parse.h>

#define PYEXEC_FORCED_EXIT (0x100)
#define PYEXEC_SWITCH_MODE (0x200)
#define PYEXEC_EXCEPTION   (0x400)
#define PYEXEC_DEEP_SLEEP  (0x800)

enum {
    EXEC_FLAG_PRINT_EOF = (1),
    EXEC_FLAG_ALLOW_DEBUGGING = (2),
    EXEC_FLAG_IS_REPL = (4),
    EXEC_FLAG_SOURCE_IS_RAW_CODE = (8),
    EXEC_FLAG_SOURCE_IS_VSTR = (16),
    EXEC_FLAG_SOURCE_IS_FILENAME = (32)
};

typedef struct {
    int return_code;
    const mp_obj_type_t * exception_type;
    int exception_line;
} pyexec_result_t;

int parse_compile_execute(const void *source, mp_parse_input_kind_t input_kind, int exec_flags, pyexec_result_t *result);
