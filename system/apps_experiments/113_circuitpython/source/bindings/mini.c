#include <stdint.h>

#include "py/obj.h"
#include "py/runtime.h"
#include "py/objproperty.h"
#include "mini.h"

mp_obj_t mini_print(size_t n_args, const mp_obj_t *args);
STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(mini_print_obj, 3, 3, mini_print);
extern mp_obj_t mini_bar(size_t n_args, const mp_obj_t *args);
STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(mini_bar_obj, 4, 4, mini_bar);
extern mp_obj_t mini_color_set(mp_obj_t arg);
STATIC MP_DEFINE_CONST_FUN_OBJ_1(mini_color_obj, mini_color_set);
extern mp_obj_t mini_back_set(mp_obj_t arg);
STATIC MP_DEFINE_CONST_FUN_OBJ_1(mini_back_obj, mini_back_set);
/*
extern mp_obj_t mini_color_set(mp_obj_t value);
extern mp_obj_t mini_back_set(mp_obj_t value);

MP_DEFINE_CONST_FUN_OBJ_1(mini_color_set_obj, mini_color_set);
MP_DEFINE_CONST_FUN_OBJ_1(mini_back_set_obj, mini_back_set);

const mp_obj_property_t mini_color_obj = {
    .base.type = &mp_type_property,
    .proxy = {(mp_obj_t)&mp_const_none_obj,
              (mp_obj_t)&mini_color_set_obj,
              (mp_obj_t)&mp_const_none_obj},
};

const mp_obj_property_t mini_back_obj = {
    .base.type = &mp_type_property,
    .proxy = {(mp_obj_t)&mp_const_none_obj,
              (mp_obj_t)&mini_back_set_obj,
              (mp_obj_t)&mp_const_none_obj},
};
*/


STATIC const mp_rom_map_elem_t mini_module_globals_table[] = {
    { MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_mini) },
    //{ MP_ROM_QSTR(MP_QSTR_TouchIn),   MP_ROM_PTR(&touchio_touchin_type) },
    { MP_ROM_QSTR(MP_QSTR_print), MP_ROM_PTR(&mini_print_obj)},
    { MP_ROM_QSTR(MP_QSTR_bar), MP_ROM_PTR(&mini_bar_obj)},
    { MP_ROM_QSTR(MP_QSTR_color), MP_ROM_PTR(&mini_color_obj)},
    { MP_ROM_QSTR(MP_QSTR_back), MP_ROM_PTR(&mini_back_obj)}
};

STATIC MP_DEFINE_CONST_DICT(mini_module_globals, mini_module_globals_table);

const mp_obj_module_t mini_module = {
    .base = { &mp_type_module },
    .globals = (mp_obj_dict_t*)&mini_module_globals,
};
