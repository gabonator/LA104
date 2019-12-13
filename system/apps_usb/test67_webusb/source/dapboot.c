/*
 * Copyright (c) 2016, Devan Lai
 *
 * Permission to use, copy, modify, and/or distribute this software
 * for any purpose with or without fee is hereby granted, provided
 * that the above copyright notice and this permission notice
 * appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL
 * WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE
 * AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
 * LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT,
 * NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <string.h>
#include <libopencm3/cm3/vector.h>
#include <libopencm3/usb/usbd.h>
#include <libopencm3/stm32/gpio.h>
#include <bluepill.h>
#include <logger.h>
#include "dapboot.h"
#include "target.h"
#include "usb_conf.h"
#include "dfu.h"
#include "webusb.h"
#include "winusb.h"
#include "config.h"
#include "uf2.h"
#include "backup.h"

static inline void __set_MSP(uint32_t topOfMainStack) {
    asm("msr msp, %0" : : "r" (topOfMainStack));
}

bool validate_application(void) {
    if ((*(volatile uint32_t *)APP_BASE_ADDRESS & 0x2FFE0000) == 0x20000000) {
        return true;
    }
    return false;
}

static void jump_to_application(void) __attribute__ ((noreturn));

static void jump_to_application(void) {
    vector_table_t* app_vector_table = (vector_table_t*)APP_BASE_ADDRESS;
    
    /* Use the application's vector table */
    target_relocate_vector_table();

    /* Do any necessary early setup for the application */
    target_pre_main();

    /* Initialize the application's stack pointer */
    __set_MSP((uint32_t)(app_vector_table->initial_sp_value));

    /* Jump to the application entry point */
    app_vector_table->reset();
    
    while (1);
}

uint32_t msTimer;
extern int msc_started;

int main(void) {
    bool appValid = false;
#ifdef SKIP_BOOTLOADER
    appValid = validate_application();
    if (appValid && target_get_force_app()) {
         jump_to_application();
         return 0;
    }
#endif  //  SKIP_BOOTLOADER         
    
    //  enable_debug();       //  Uncomment to allow display of debug messages in development devices. NOTE: This will hang if no debugger is attached.
    disable_debug();  //  Uncomment to disable display of debug messages.  For use in production devices.
    platform_setup();     //  STM32 platform setup.
    debug_println("----bootloader");  // debug_flush();
    
    //  target_clock_setup();  //  Clock already setup in platform_setup()
    target_gpio_setup();       //  Initialize GPIO/LEDs if needed
    // test_backup();          //  Test backup.

    debug_println("target_get_force_bootloader");  // debug_flush();
    if (target_get_force_bootloader() || !appValid) {        
        {  //  Setup USB
            char serial[USB_SERIAL_NUM_LENGTH+1];
            serial[0] = '\0';
            debug_println("target_get_serial_number");  // debug_flush();
            target_get_serial_number(serial, USB_SERIAL_NUM_LENGTH);

            debug_println("usb_set_serial_number");  // debug_flush();
            usb_set_serial_number(serial);
        }
        debug_println("usb_setup");  // debug_flush();
        usbd_device* usbd_dev = usb_setup();
        debug_println("usbd polling...");  debug_flush();  ////
        uint32_t cycleCount = 0;        
        while (1) {
            cycleCount++;
            if (cycleCount >= 700) {
                msTimer++;
                cycleCount = 0;

                int v = msTimer % 500;
                target_set_led(v < 50);

                ghostfat_1ms();

                if (appValid && !msc_started && msTimer > 1000) {
                    debug_println("target_manifest_app");  debug_flush();
                    target_manifest_app();
                }
            }

            usbd_poll(usbd_dev);
        }
    } else {
        debug_println("jump_to_application");  debug_flush();
        jump_to_application();
    }    
    return 0;
}

#ifdef NOTUSED
static void test_backup(void) {
    //  Test whether RTC backup registers are written correctly.
    //  static const uint32_t CMD_BOOT = 0x544F4F42UL;
    //  backup_write(BKP0, CMD_BOOT);  //  Uncomment to force booting to bootloader.

    uint32_t val = backup_read(BKP0);
    debug_print("read bkp0 "); debug_print_unsigned((size_t) val); debug_println(""); debug_flush();

    enum BackupRegister reg = BKP1;
    uint32_t cmd = backup_read(reg);
    debug_print("test_backup read "); debug_print_unsigned((size_t) cmd); debug_println(""); debug_flush();
    cmd++;
    backup_write(reg, cmd);
    debug_print("test_backup write "); debug_print_unsigned((size_t) cmd); debug_println(""); debug_flush();
    cmd = backup_read(reg);
    debug_print("test_backup read again "); debug_print_unsigned((size_t) cmd); debug_println(""); debug_flush();
}
#endif  //  NOTUSED
