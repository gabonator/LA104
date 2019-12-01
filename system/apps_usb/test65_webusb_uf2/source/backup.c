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

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/rtc.h>
#include <libopencm3/stm32/pwr.h>

#include "backup.h"

#define RTC_BKP_DR(reg)  MMIO16(BACKUP_REGS_BASE + 4 + (4 * (reg)))

void backup_write(enum BackupRegister reg, uint32_t value) {
    /*
    rcc_periph_clock_enable(RCC_PWR);
    rcc_periph_clock_enable(RCC_BKP);

    pwr_disable_backup_domain_write_protect();
    RTC_BKP_DR((int)reg*2) = value & 0xFFFFUL;
    RTC_BKP_DR((int)reg*2+1) = (value & 0xFFFF0000UL) >> 16;
    pwr_enable_backup_domain_write_protect();
    */
   (void)reg;
    *(volatile uint32_t*)0x20004000 = value;
}

uint32_t backup_read(enum BackupRegister reg) {
   (void)reg;
    return *(volatile uint32_t*)0x20004000;
    /*
    uint32_t value = ((uint32_t)RTC_BKP_DR((int)reg*2+1) << 16)
                   | ((uint32_t)RTC_BKP_DR((int)reg*2) << 0);
    return value;
    */
}
