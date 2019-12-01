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

/* Common STM32F103 target functions */

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/st_usbfs.h>
#include <libopencm3/stm32/flash.h>
#include <libopencm3/stm32/desig.h>
#include <libopencm3/cm3/scb.h>

#include "target.h"
#include "config.h"
#include "backup.h"

#ifndef USES_GPIOA
#if (HAVE_USB_PULLUP_CONTROL == 0)
#define USES_GPIOA 1
#else
#define USES_GPIOA 0
#endif
#endif

#ifndef USES_GPIOB
#define USES_GPIOB 0
#endif

#ifndef USES_GPIOC
#define USES_GPIOC 0
#endif

#ifdef FLASH_SIZE_OVERRIDE
_Static_assert((FLASH_BASE + FLASH_SIZE_OVERRIDE >= APP_BASE_ADDRESS),
               "Incompatible flash size");
#endif

static const uint32_t CMD_BOOT = 0x544F4F42UL;
static const uint32_t CMD_APP = 0x3f82722aUL;

//#define USE_HSI 1

void target_clock_setup(void) {
#ifdef USE_HSI
    /* Set the system clock to 48MHz from the internal RC oscillator.
       The clock tolerance doesn't meet the official USB spec, but
       it's better than nothing. */
    rcc_clock_setup_in_hsi_out_48mhz();
#else
    /* Set system clock to 72 MHz from an external crystal */
    #ifdef CRYSTAL_16MHZ
    rcc_clock_setup_in_hse_16mhz_out_72mhz();
    #else
    rcc_clock_setup_in_hse_8mhz_out_72mhz();
    #endif
#endif
}

void target_set_led(int on) {
#if HAVE_LED
        if ((on && LED_OPEN_DRAIN) || (!on && !LED_OPEN_DRAIN)) {
            gpio_clear(LED_GPIO_PORT, LED_GPIO_PIN);
        } else {
            gpio_set(LED_GPIO_PORT, LED_GPIO_PIN);
        }
#else
    (void)on;
#endif
}

static void sleep_us(int us){
    for (int i = 0; i < us*10; i++) {
        __asm__("nop");
    }
}

void target_gpio_setup(void) {
    /* Enable GPIO clocks */
    rcc_periph_clock_enable(RCC_GPIOA);
    rcc_periph_clock_enable(RCC_GPIOB);
    rcc_periph_clock_enable(RCC_GPIOC);

    /* Setup LEDs */
#if HAVE_LED
    {
        const uint8_t mode = GPIO_MODE_OUTPUT_10_MHZ;
        const uint8_t conf = (LED_OPEN_DRAIN ? GPIO_CNF_OUTPUT_OPENDRAIN
                                             : GPIO_CNF_OUTPUT_PUSHPULL);
        if (LED_OPEN_DRAIN) {
            gpio_set(LED_GPIO_PORT, LED_GPIO_PIN);
        } else {
            gpio_clear(LED_GPIO_PORT, LED_GPIO_PIN);
        }
        gpio_set_mode(LED_GPIO_PORT, mode, conf, LED_GPIO_PIN);
    }
#endif

    /* Setup the internal pull-up/pull-down for the button */
#if HAVE_BUTTON
    {
        const uint8_t mode = GPIO_MODE_INPUT;
        const uint8_t conf = GPIO_CNF_INPUT_PULL_UPDOWN;
        gpio_set_mode(BUTTON_GPIO_PORT, mode, conf, BUTTON_GPIO_PIN);
        if (BUTTON_ACTIVE_HIGH) {
            gpio_clear(BUTTON_GPIO_PORT, BUTTON_GPIO_PIN);
        } else {
            gpio_set(BUTTON_GPIO_PORT, BUTTON_GPIO_PIN);
        }
    }
#endif

#if HAVE_USB_PULLUP_CONTROL
    {
        const uint8_t mode = GPIO_MODE_OUTPUT_10_MHZ;
        const uint8_t conf = (USB_PULLUP_OPEN_DRAIN ? GPIO_CNF_OUTPUT_OPENDRAIN
                                                    : GPIO_CNF_OUTPUT_PUSHPULL);
        /* Configure USB pullup transistor, initially disabled */
        if (USB_PULLUP_ACTIVE_HIGH) {
            gpio_clear(USB_PULLUP_GPIO_PORT, USB_PULLUP_GPIO_PIN);
        } else {
            gpio_set(USB_PULLUP_GPIO_PORT, USB_PULLUP_GPIO_PIN);
        }
        gpio_set_mode(USB_PULLUP_GPIO_PORT, mode, conf, USB_PULLUP_GPIO_PIN);
    }
#else
    {
        /* Drive the USB DP pin to override the pull-up */
        gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_10_MHZ,
                      GPIO_CNF_OUTPUT_PUSHPULL, GPIO12);
    }
#endif

#if 0
    while(1) {
        target_set_led(1);
        sleep_us(1200000);
        target_set_led(0);
        sleep_us(1200000);
    }

    // TFT
    // RST
    gpio_clear(GPIOC, (1 << 4));
    sleep_us(20000);
    //gpio_set(GPIOC, (1 << 4));
    sleep_us(20000);
#endif
}

const usbd_driver* target_usb_init(void) {
    rcc_periph_reset_pulse(RST_USB);

#if HAVE_USB_PULLUP_CONTROL
    /* Enable USB pullup to connect */
    if (USB_PULLUP_ACTIVE_HIGH) {
        gpio_set(USB_PULLUP_GPIO_PORT, USB_PULLUP_GPIO_PIN);
    } else {
        gpio_clear(USB_PULLUP_GPIO_PORT, USB_PULLUP_GPIO_PIN);
    }
#else
    /* Override hard-wired USB pullup to disconnect and reconnect */
    gpio_clear(GPIOA, GPIO12);
    int i;
    for (i = 0; i < 800000; i++) {
        __asm__("nop");
    }
#endif

    return &st_usbfs_v1_usb_driver;
}

void target_manifest_app(void) {
    backup_write(BKP0, CMD_APP);
    scb_reset_system();
}

bool target_get_force_app(void) {
    if (backup_read(BKP0) == CMD_APP) {
        backup_write(BKP0, 0);
        return true;        
    }
    return false;
}

bool target_get_force_bootloader(void) {
    /* Enable GPIO clocks */
    rcc_periph_clock_enable(RCC_GPIOA);
    rcc_periph_clock_enable(RCC_GPIOB);
    rcc_periph_clock_enable(RCC_GPIOC);

    bool force = true;
    /* Check the RTC backup register */
    uint32_t cmd = backup_read(BKP0);
    if (cmd == CMD_BOOT) {
        // asked to go into bootloader?
        backup_write(BKP0, 0);
        return true;
    }
    if (cmd == CMD_APP) {        
        // we were told to reset into app
        backup_write(BKP0, 0);
        return false;
    }

#ifdef DOUBLE_TAP
    target_set_led(1);
    // wait for second press on reset
    backup_write(BKP0, CMD_BOOT);
    for (int i = 0; i < 3500000; ++i)
        asm("nop");
    backup_write(BKP0, 0);
    target_set_led(0);
    force = false;
#else
    // a reset now should go into app
    backup_write(BKP0, CMD_APP);
#endif

#if HAVE_BUTTON
    /* Check if the user button is held down */
    if (BUTTON_ACTIVE_HIGH) {
        if (gpio_get(BUTTON_GPIO_PORT, BUTTON_GPIO_PIN)) {
            force = true;
        }
    } else {
        if (!gpio_get(BUTTON_GPIO_PORT, BUTTON_GPIO_PIN)) {
            force = true;
        }
    }
#endif

    return force;
}

void target_get_serial_number(char* dest, size_t max_chars) {
    desig_get_unique_id_as_string(dest, max_chars+1);
}

static uint16_t* get_flash_end(void) {
#ifdef FLASH_SIZE_OVERRIDE
    /* Allow access to the unofficial full 128KiB flash size */
    return (uint16_t*)(FLASH_BASE + FLASH_SIZE_OVERRIDE);
#else
    /* Only allow access to the chip's self-reported flash size */
    return (uint16_t*)(FLASH_BASE + (size_t)DESIG_FLASH_SIZE*FLASH_PAGE_SIZE);
#endif
}

size_t target_get_max_firmware_size(void) {
    uint8_t* flash_end = (uint8_t*)get_flash_end();
    uint8_t* flash_start = (uint8_t*)(APP_BASE_ADDRESS);

    return (flash_end >= flash_start) ? (size_t)(flash_end - flash_start) : 0;
}

void target_relocate_vector_table(void) {
    SCB_VTOR = APP_BASE_ADDRESS & 0xFFFF;
}

void target_flash_unlock(void) {
    flash_unlock();
}

void target_flash_lock(void) {
    flash_lock();
}

static inline uint16_t* get_flash_page_address(uint16_t* dest) {
    return (uint16_t*)(((uint32_t)dest / FLASH_PAGE_SIZE) * FLASH_PAGE_SIZE);
}

bool target_flash_program_array(uint16_t* dest, const uint16_t* data, size_t half_word_count) {
    bool verified = true;

    /* Remember the bounds of erased data in the current page */
    static uint16_t* erase_start;
    static uint16_t* erase_end;

    const uint16_t* flash_end = get_flash_end();
    while (half_word_count > 0) {
        /* Avoid writing past the end of flash */
        if (dest >= flash_end) {
            verified = false;
            break;
        }

        if (dest >= erase_end || dest < erase_start) {
            erase_start = get_flash_page_address(dest);
            erase_end = erase_start + (FLASH_PAGE_SIZE)/sizeof(uint16_t);
            flash_erase_page((uint32_t)erase_start);
        }
        flash_program_half_word((uint32_t)dest, *data);
        erase_start = dest + 1;
        if (*dest != *data) {
            verified = false;
            break;
        }
        dest++;
        data++;
        half_word_count--;
    }

    return verified;
}
