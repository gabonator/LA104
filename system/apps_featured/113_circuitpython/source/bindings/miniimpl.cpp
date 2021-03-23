#include <library.h>
//#include "mini.h"
extern "C"
{
#include <py/runtime.h>
#include <py/obj.h>
}
#include "draw.h"
int lcdColor = 0xffff;
int lcdBack = 0x0000;
bool usingKeys = false;

extern "C"  {
int mini_color(uint32_t c)
{
    return RGB565RGB(GetHtmlR(c), GetHtmlG(c), GetHtmlB(c));
}

mp_obj_t mini_lcd_color_set(mp_obj_t self_in, mp_obj_t value)
{
    uint32_t color = mp_obj_get_int(value);
    lcdColor = mini_color(color);
    return mp_const_none;
}

mp_obj_t mini_lcd_back_set(mp_obj_t self_in, mp_obj_t value)
{
    uint32_t color = mp_obj_get_int(value);
    lcdBack = mini_color(color);
    return mp_const_none;
}

mp_obj_t mini_lcd_print(size_t n_args, const mp_obj_t *args) {
    (void)n_args;

    mp_int_t x, y;
    if (!mp_obj_get_int_maybe(args[1], &x) ||
        !mp_obj_get_int_maybe(args[2], &y) ||
        !MP_OBJ_IS_STR(args[3]))
    {
        mp_raise_ValueError(translate("wrong args"));
        return mp_const_none;
    }

    size_t len = 0;
    const char* msg = mp_obj_str_get_data(args[2], &len);

    BIOS::LCD::Print(x, y, lcdColor, lcdBack, msg);
    return mp_const_none;
}

mp_obj_t mini_lcd_bar(size_t n_args, const mp_obj_t *args) {
    (void)n_args;
    mp_int_t x1, y1, x2, y2;
    if (!mp_obj_get_int_maybe(args[1], &x1) ||
        !mp_obj_get_int_maybe(args[2], &y1) ||
        !mp_obj_get_int_maybe(args[3], &x2) ||
        !mp_obj_get_int_maybe(args[4], &y2))
    {
        mp_raise_ValueError(translate("wrong args"));
        return mp_const_none;
    }
    BIOS::LCD::Bar(x1, y1, x2, y2, lcdColor);
    return mp_const_none;
}

mp_obj_t mini_lcd_line(size_t n_args, const mp_obj_t *args) {
    (void)n_args;
    float x1, y1, x2, y2;
    if (!mp_obj_get_float_maybe(args[1], &x1) ||
        !mp_obj_get_float_maybe(args[2], &y1) ||
        !mp_obj_get_float_maybe(args[3], &x2) ||
        !mp_obj_get_float_maybe(args[4], &y2))
    {
        mp_raise_ValueError(translate("wrong args"));
        return mp_const_none;
    }
    
    drawline_aa((int)(x1*256), (int)(y1*256), (int)(x2*256), (int)(y2*256), lcdColor);

    return mp_const_none;
}

mp_obj_t mini_lcd_get_width(mp_obj_t self_in) {
    //touchio_touchin_obj_t *self = MP_OBJ_TO_PTR(self_in);
    //check_for_deinit(self);
    return mp_obj_new_int(BIOS::LCD::Width);
}

mp_obj_t mini_lcd_get_height(mp_obj_t self_in) {
    //touchio_touchin_obj_t *self = MP_OBJ_TO_PTR(self_in);
    //check_for_deinit(self);
    return mp_obj_new_int(BIOS::LCD::Height);
}

mp_obj_t mini_key_get(mp_obj_t self_in)
{
    usingKeys = true;
    return mp_obj_new_int(BIOS::KEY::GetKey());
}

mp_obj_t mini_sys_delay(mp_obj_t self, mp_obj_t value)
{
    uint32_t ms = mp_obj_get_int(value);
    BIOS::SYS::DelayMs(ms);
    return mp_const_none;
}

}
