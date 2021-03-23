#include "exec.h"
#include "py/compile.h"
#include "py/runtime.h"
#include "py/repl.h"
#include "py/gc.h"
#include "py/gc_long_lived.h"
#include "py/frozenmod.h"
#include "py/mphal.h"

#include "lib/utils/interrupt_char.h"
#include "lib/mp-readline/readline.h"

int pyexec_system_exit = 0;

#if MICROPY_DEBUG_VERBOSE // print debugging info
#define DEBUG_PRINT (1)
#else // don't print debugging info
#define DEBUG_PRINT (0)
#define DEBUG_printf(...) (void)0
#endif

#define EXEC_FLAG_PRINT_EOF (1)
#define EXEC_FLAG_ALLOW_DEBUGGING (2)
#define EXEC_FLAG_IS_REPL (4)
#define EXEC_FLAG_SOURCE_IS_RAW_CODE (8)
#define EXEC_FLAG_SOURCE_IS_VSTR (16)
#define EXEC_FLAG_SOURCE_IS_FILENAME (32)

// parses, compiles and executes the code in the lexer
// frees the lexer before returning
// EXEC_FLAG_PRINT_EOF prints 2 EOF chars: 1 after normal output, 1 after exception output
// EXEC_FLAG_ALLOW_DEBUGGING allows debugging info to be printed after executing the code
// EXEC_FLAG_IS_REPL is used for REPL inputs (flag passed on to mp_compile)
int parse_compile_execute(const void *source, mp_parse_input_kind_t input_kind, int exec_flags, pyexec_result_t *result) {
    int ret = 0;
    // by default a SystemExit exception returns 0
    pyexec_system_exit = 0;

    nlr_buf_t nlr;
    if (nlr_push(&nlr) == 0) {
        mp_obj_t module_fun;
        //gabo
        if (exec_flags & EXEC_FLAG_SOURCE_IS_RAW_CODE) {
            // source is a raw_code object, create the function
            module_fun = mp_make_function_from_raw_code(source, MP_OBJ_NULL, MP_OBJ_NULL);
        } else
        {
            #if MICROPY_ENABLE_COMPILER
            mp_lexer_t *lex;
            if (exec_flags & EXEC_FLAG_SOURCE_IS_VSTR) {
                const vstr_t *vstr = source;
                lex = mp_lexer_new_from_str_len(MP_QSTR__lt_stdin_gt_, vstr->buf, vstr->len, 0);
            } else if (exec_flags & EXEC_FLAG_SOURCE_IS_FILENAME) {
                lex = mp_lexer_new_from_file(source);
            } else {
                lex = (mp_lexer_t*)source;
            }
            // source is a lexer, parse and compile the script
            qstr source_name = lex->source_name;
            if (input_kind == MP_PARSE_FILE_INPUT) {
                mp_store_global(MP_QSTR___file__, MP_OBJ_NEW_QSTR(source_name));
            }
            mp_parse_tree_t parse_tree = mp_parse(lex, input_kind);
            module_fun = mp_compile(&parse_tree, source_name, MP_EMIT_OPT_NONE, exec_flags & EXEC_FLAG_IS_REPL);
            // Clear the parse tree because it has a heap pointer we don't need anymore.
            *((void* volatile*) &parse_tree.chunk) = 0;
            #else
            mp_raise_msg(&mp_type_RuntimeError, translate("script compilation not supported"));
            #endif
        }

        // If the code was loaded from a file it's likely to be running for a while so we'll long
        // live it and collect any garbage before running.
        if (input_kind == MP_PARSE_FILE_INPUT) {
            module_fun = make_obj_long_lived(module_fun, 6);
            gc_collect();
        }

        // execute code
        mp_hal_set_interrupt_char(CHAR_CTRL_C); // allow ctrl-C to interrupt us
        mp_call_function_0(module_fun);
        mp_hal_set_interrupt_char(-1); // disable interrupt
        // Handle any ctrl-c interrupt that arrived just in time
        mp_handle_pending();
        nlr_pop();
        ret = 0;
    } else {
        // uncaught exception
        // FIXME it could be that an interrupt happens just before we disable it here
        mp_hal_set_interrupt_char(-1); // disable interrupt
        // print EOF after normal output
        // check for SystemExit
        if (mp_obj_is_subclass_fast(mp_obj_get_type((mp_obj_t)nlr.ret_val), &mp_type_SystemExit)) {
            // at the moment, the value of SystemExit is unused
            ret = pyexec_system_exit;
#if CIRCUITPY_ALARM
        } else if (mp_obj_is_subclass_fast(mp_obj_get_type((mp_obj_t)nlr.ret_val), &mp_type_DeepSleepRequest)) {
            ret = PYEXEC_DEEP_SLEEP;
#endif
        } else {
            if ((mp_obj_t) nlr.ret_val != MP_OBJ_FROM_PTR(&MP_STATE_VM(mp_reload_exception))) {
                mp_obj_print_exception(&mp_plat_print, (mp_obj_t)nlr.ret_val);
            }
            ret = PYEXEC_EXCEPTION;
        }
    }
    if (result != NULL) {
        result->return_code = ret;
        if (ret != 0) {
            mp_obj_t return_value = (mp_obj_t)nlr.ret_val;
            result->exception_type = mp_obj_get_type(return_value);
            result->exception_line = -1;

            if (mp_obj_is_exception_instance(return_value)) {
                size_t n, *values;
                mp_obj_exception_get_traceback(return_value, &n, &values);
                if (values != NULL) {
                    result->exception_line = values[n - 2];
                }
            }
        }
    }

    return ret;
}
