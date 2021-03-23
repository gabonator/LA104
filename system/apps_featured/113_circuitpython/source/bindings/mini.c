#include <stdint.h>

#include "py/obj.h"
#include "py/runtime.h"
#include "py/objproperty.h"
#include "mini.h"
#include "lib/utils/context_manager_helpers.h"

// lcd

mp_obj_t mini_lcd_print(size_t n_args, const mp_obj_t *args);
STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(mini_lcd_print_obj, 4, 4, mini_lcd_print);
extern mp_obj_t mini_lcd_bar(size_t n_args, const mp_obj_t *args);
STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(mini_lcd_bar_obj, 5, 5, mini_lcd_bar);
extern mp_obj_t mini_lcd_line(size_t n_args, const mp_obj_t *args);
STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(mini_lcd_line_obj, 5, 5, mini_lcd_line);
extern mp_obj_t mini_lcd_color_set(mp_obj_t self_in, mp_obj_t value);
STATIC MP_DEFINE_CONST_FUN_OBJ_2(mini_lcd_color_obj, mini_lcd_color_set);
extern mp_obj_t mini_lcd_back_set(mp_obj_t self_in, mp_obj_t value);
STATIC MP_DEFINE_CONST_FUN_OBJ_2(mini_lcd_back_obj, mini_lcd_back_set);

extern mp_obj_t mini_lcd_get_width(mp_obj_t self);
MP_DEFINE_CONST_FUN_OBJ_1(mini_lcd_get_width_obj, mini_lcd_get_width);

const mp_obj_property_t mini_lcd_width_obj = {
    .base.type = &mp_type_property,
    .proxy = {(mp_obj_t)&mini_lcd_get_width_obj,
              (mp_obj_t)&mp_const_none_obj,
              (mp_obj_t)&mp_const_none_obj},
};

extern mp_obj_t mini_lcd_get_height(mp_obj_t self);
MP_DEFINE_CONST_FUN_OBJ_1(mini_lcd_get_height_obj, mini_lcd_get_height);

const mp_obj_property_t mini_lcd_height_obj = {
    .base.type = &mp_type_property,
    .proxy = {(mp_obj_t)&mini_lcd_get_height_obj,
              (mp_obj_t)&mp_const_none_obj,
              (mp_obj_t)&mp_const_none_obj},
};

STATIC const mp_rom_map_elem_t mini_lcd_locals_dict_table[] = {
    { MP_ROM_QSTR(MP_QSTR_width), MP_ROM_PTR(&mini_lcd_width_obj)},
    { MP_ROM_QSTR(MP_QSTR_height), MP_ROM_PTR(&mini_lcd_height_obj)},
    { MP_ROM_QSTR(MP_QSTR_print), MP_ROM_PTR(&mini_lcd_print_obj)},
    { MP_ROM_QSTR(MP_QSTR_bar), MP_ROM_PTR(&mini_lcd_bar_obj)},
    { MP_ROM_QSTR(MP_QSTR_line), MP_ROM_PTR(&mini_lcd_line_obj)},
    { MP_ROM_QSTR(MP_QSTR_color), MP_ROM_PTR(&mini_lcd_color_obj)},
    { MP_ROM_QSTR(MP_QSTR_back), MP_ROM_PTR(&mini_lcd_back_obj)}
};

STATIC MP_DEFINE_CONST_DICT(mini_lcd_locals_dict, mini_lcd_locals_dict_table);
STATIC mp_obj_t mini_lcd_make_new(const mp_obj_type_t *type, mp_uint_t n_args, const mp_obj_t *args, mp_map_t *kw_args);

const mp_obj_type_t mini_lcd_type = {
    { &mp_type_type },
    .name = MP_QSTR_lcd,
    .make_new = mini_lcd_make_new,
    .locals_dict = (mp_obj_t)&mini_lcd_locals_dict,
};

STATIC mp_obj_t mini_lcd_make_new(const mp_obj_type_t *type,
        mp_uint_t n_args, const mp_obj_t *args, mp_map_t *kw_args) {
    mp_arg_check_num(n_args, kw_args, 0, 0, false);
    mp_obj_base_t *self = m_new_obj(mp_obj_base_t);
    self->type = &mini_lcd_type;
    return (mp_obj_t) self;
}


// key

extern mp_obj_t mini_key_get(mp_obj_t self);
STATIC MP_DEFINE_CONST_FUN_OBJ_1(mini_key_get_obj, mini_key_get);

STATIC const mp_rom_map_elem_t mini_key_locals_dict_table[] = {
    { MP_ROM_QSTR(MP_QSTR_get), MP_ROM_PTR(&mini_key_get_obj)},
};

STATIC MP_DEFINE_CONST_DICT(mini_key_locals_dict, mini_key_locals_dict_table);

STATIC mp_obj_t mini_key_make_new(const mp_obj_type_t *type, mp_uint_t n_args, const mp_obj_t *args, mp_map_t *kw_args);

const mp_obj_type_t mini_key_type = {
    { &mp_type_type },
    .name = MP_QSTR_key,
    .make_new = mini_key_make_new,
    .locals_dict = (mp_obj_t)&mini_key_locals_dict,
};

STATIC mp_obj_t mini_key_make_new(const mp_obj_type_t *type,
        mp_uint_t n_args, const mp_obj_t *args, mp_map_t *kw_args) {
    //mp_arg_check_num(n_args, kw_args, 0, 0, false);
    mp_obj_base_t *self = m_new_obj(mp_obj_base_t);
    self->type = &mini_key_type;
    return (mp_obj_t) self;
}

// sys

extern mp_obj_t mini_sys_delay(mp_obj_t self, mp_obj_t value);
STATIC MP_DEFINE_CONST_FUN_OBJ_2(mini_sys_delay_obj, mini_sys_delay);

STATIC const mp_rom_map_elem_t mini_sys_locals_dict_table[] = {
    { MP_ROM_QSTR(MP_QSTR_delay), MP_ROM_PTR(&mini_sys_delay_obj)},
};

STATIC MP_DEFINE_CONST_DICT(mini_sys_locals_dict, mini_sys_locals_dict_table);

STATIC mp_obj_t mini_sys_make_new(const mp_obj_type_t *type, mp_uint_t n_args, const mp_obj_t *args, mp_map_t *kw_args);

const mp_obj_type_t mini_sys_type = {
    { &mp_type_type },
    .name = MP_QSTR_key,
    .make_new = mini_sys_make_new,
    .locals_dict = (mp_obj_t)&mini_sys_locals_dict,
};

STATIC mp_obj_t mini_sys_make_new(const mp_obj_type_t *type,
        mp_uint_t n_args, const mp_obj_t *args, mp_map_t *kw_args) {
    //mp_arg_check_num(n_args, kw_args, 0, 0, false);
    mp_obj_base_t *self = m_new_obj(mp_obj_base_t);
    self->type = &mini_sys_type;
    return (mp_obj_t) self;
}


// root
STATIC const mp_rom_map_elem_t mini_module_globals_table[] = {
    { MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_mini) },
    
    { MP_ROM_QSTR(MP_QSTR_lcd),   MP_ROM_PTR(&mini_lcd_type) }, // class
    { MP_ROM_QSTR(MP_QSTR_key),   MP_ROM_PTR(&mini_key_type) }, // class
    { MP_ROM_QSTR(MP_QSTR_sys),   MP_ROM_PTR(&mini_sys_type) }, // class
};

STATIC MP_DEFINE_CONST_DICT(mini_module_globals, mini_module_globals_table);

const mp_obj_module_t mini_module = {
    .base = { &mp_type_module },
    .globals = (mp_obj_dict_t*)&mini_module_globals,
};
