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

#include "uf2.h"
#include "dapboot.h"
#include "target.h"
#include "usb_conf.h"
#include "webusb.h"
#include "winusb.h"
#include "config.h"

#include <libopencm3/usb/msc.h>

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
    bool appValid = validate_application();

    if (appValid && target_get_force_app()) {
         jump_to_application();
         return 0;
    }
    
    /* Setup clocks */
    target_clock_setup();

    /* Initialize GPIO/LEDs if needed */
    target_gpio_setup();

    if (target_get_force_bootloader() || !appValid) {
        /* Setup USB */
        {
            char serial[USB_SERIAL_NUM_LENGTH+1];
            serial[0] = '\0';
            target_get_serial_number(serial, USB_SERIAL_NUM_LENGTH);
            usb_set_serial_number(serial);
        }

        usbd_device* usbd_dev = usb_setup();
        //dfu_setup(usbd_dev, &target_manifest_app, NULL, NULL);
       	usb_msc_init(usbd_dev, 0x82, 64, 0x01, 64, "Example Ltd", "UF2 Bootloader",
		    "42.00", UF2_NUM_BLOCKS, read_block, write_block);
        winusb_setup(usbd_dev);

        int cycleCount = 0;
        int br = 500;
        int d = 1;
        
        while (1) {
            cycleCount++;
            
            target_set_led(cycleCount < br);

            if (cycleCount >= 700) {
                msTimer++;
                cycleCount = 0;

                br += d;
                if (br > 700)
                    d = -2;
                else if (br < 10)
                    d = 2;

                //int v = msTimer % 500;
                //target_set_led(v < 50);

                ghostfat_1ms();

                if (appValid && !msc_started && msTimer > 3000) {
                    target_manifest_app();
                }
            }

            usbd_poll(usbd_dev);
        }
    } else {
        jump_to_application();
    }
    
    return 0;
}
