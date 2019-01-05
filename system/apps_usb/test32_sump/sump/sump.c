/*
 *
 * SUMP Protocol Implementation for Arduino boards.
 *
 * Copyright (c) 2011,2012,2013,2014,2015 Andrew Gillham
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY ANDREW GILLHAM ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL ANDREW GILLHAM BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *
 */

// this code is based on:
// https://github.com/gillham/logic_analyzer/blob/master/logic_analyzer.ino
// modified to work under stm32 HAL

//#include "stm32f1xx_hal.h"
#include "sump.h"
#include "gpio.h"

#include <string.h>

#define SUMP_RESET 0x00
#define SUMP_ARM   0x01
#define SUMP_QUERY 0x02
#define SUMP_XON   0x11
#define SUMP_XOFF  0x13

/* mask & values used, config ignored. only stage0 supported */
#define SUMP_TRIGGER_MASK 0xC0
#define SUMP_TRIGGER_VALUES 0xC1
#define SUMP_TRIGGER_CONFIG 0xC2

/* Most flags (except RLE) are ignored. */
#define SUMP_SET_DIVIDER 0x80
#define SUMP_SET_READ_DELAY_COUNT 0x81
#define SUMP_SET_FLAGS 0x82
#define SUMP_SET_RLE 0x0100

/* extended commands -- self-test unsupported, but metadata is returned. */
#define SUMP_SELF_TEST 0x03
#define SUMP_GET_METADATA 0x04

typedef enum {
    STATE_READ4 = 0,
    STATE_READ3 = 1,
    STATE_READ2 = 2,
    STATE_READ1 = 3,
    STATE_EXTENDED_CMD = 4,
    STATE_CMD = 5
} sump_cmd_state;

static uint8_t run_gpio = 0;

const static uint8_t metadata[] = {
  1, 'A', 'G', 'L', 'A', 'S', 'v', '0', 0, // device name
  2, '0', '.', '1', '3', 0,                // firmware version
  0x21, 0, 0, 20, 0,                       // sample memory = 40*256 = 10240 bytes   //gabo: reduced to 20*256
  0x23, 0, 0x5B, 0x8D, 80,                 // sample rate (6MHz)
  0x40, 4,                                 // number of probes = 4
  0x41, 2,                                 // protocol version 2
  0                                        // end of data
};

static void get_metadata() {
  CDC_Transmit_FS((uint8_t *)metadata, sizeof(metadata));
}

static uint8_t trigger, trigger_values, rleEnabled;
static uint32_t readCount, delayCount, divider = 11;

void extended_sump_command(char last_cmd, uint8_t *extended_cmd_arg) {
/*
  write_uart_s("e ");
  write_uart_u(last_cmd);
  write_uart_s(", ");
  write_uart_u(extended_cmd_arg[0]);
  write_uart_s(", ");
  write_uart_u(extended_cmd_arg[1]);
  write_uart_s(", ");
  write_uart_u(extended_cmd_arg[2]);
  write_uart_s(", ");
  write_uart_u(extended_cmd_arg[3]);
  write_uart_s("\n");
*/

  switch (last_cmd) {
    case SUMP_TRIGGER_MASK:
      /*
       * the trigger mask byte has a '1' for each enabled trigger so
       * we can just use it directly as our trigger mask.
       */
      trigger = extended_cmd_arg[0];
      write_uart_s("trigger ");
      write_uart_u(trigger);
      write_uart_s("\n");
      break;
    case SUMP_TRIGGER_VALUES:
      /*
       * trigger_values can be used directly as the value of each bit
       * defines whether we're looking for it to be high or low.
       */
      trigger_values = extended_cmd_arg[0];
      write_uart_s("t_values ");
      write_uart_u(trigger_values);
      write_uart_s("\n");
      break;
    case SUMP_TRIGGER_CONFIG:
      write_uart_s("t_config = ?\n");
      /* read the rest of the command bytes, but ignore them. */
      break;
    case SUMP_SET_DIVIDER:
      /*
       * the shifting needs to be done on the 32bit unsigned long variable
       * so that << 16 doesn't end up as zero.
       */
      divider = extended_cmd_arg[2];
      divider = divider << 8;
      divider += extended_cmd_arg[1];
      divider = divider << 8;
      divider += extended_cmd_arg[0];
      write_uart_s("divider ");
      write_uart_u(divider);
      write_uart_s("\n");
//      setupDelay(divider);
      break;
    case SUMP_SET_READ_DELAY_COUNT:
      /*
       * this just sets up how many samples there should be before
       * and after the trigger fires.  The readCount is total samples
       * to return and delayCount number of samples after the trigger.
       * this sets the buffer splits like 0/100, 25/75, 50/50
       * for example if readCount == delayCount then we should
       * return all samples starting from the trigger point.
       * if delayCount < readCount we return (readCount - delayCount) of
       * samples from before the trigger fired.
       */
      readCount = 4 * (((extended_cmd_arg[1] << 8) | extended_cmd_arg[0]) + 1);
      if (readCount > GPIO_BUFFER_SIZE)
        readCount = GPIO_BUFFER_SIZE;
      write_uart_s("read# ");
      write_uart_u(readCount);
      write_uart_s("\n");
      delayCount = 4 * (((extended_cmd_arg[3] << 8) | extended_cmd_arg[2]) + 1);
      if (delayCount > GPIO_BUFFER_SIZE)
        delayCount = GPIO_BUFFER_SIZE;
      write_uart_s("delay# ");
      write_uart_u(delayCount);
      write_uart_s("\n");
      break; // TODO
    case SUMP_SET_FLAGS:
      /* read the rest of the command bytes and check if RLE is enabled. */
      rleEnabled = ((extended_cmd_arg[1] & 0b1000000) != 0);
      write_uart_s("rle ");
      write_uart_u(rleEnabled);
      write_uart_s("\n");
      break; // TODO
  }
}

void sump_read_command(sump_cmd_state *read_state, char c) {
/*
  write_uart_s("c ");
  write_uart_u(c);
  write_uart_s("\n");
*/
  switch (c) {
    case SUMP_RESET:
      /*
       * We don't do anything here as some unsupported extended commands have
       * zero bytes and are mistaken as resets.  This can trigger false resets
       * so we don't erase the data or do anything for a reset.
       */
      break;
    case SUMP_QUERY:
      /* return the expected bytes. */
      {
        static const char* query = "1ALS";
        CDC_Transmit_FS((uint8_t*)query, strlen(query));
      }
      break;
    case SUMP_ARM:
      /*
       * Zero out any previous samples before arming.
       * Done here instead via reset due to spurious resets.
       */
      memset(gpio_buffer, '\0', GPIO_BUFFER_SIZE);
      run_gpio = 1;
      break;
    case SUMP_TRIGGER_MASK:
    case SUMP_TRIGGER_VALUES:
    case SUMP_TRIGGER_CONFIG:
    case SUMP_SET_DIVIDER:
    case SUMP_SET_READ_DELAY_COUNT:
    case SUMP_SET_FLAGS:
      // extended commands have a 4 byte argument
      *read_state = STATE_READ4;
      break;
    case SUMP_GET_METADATA:
      /*
       * We return a description of our capabilities.
       * Check the function's comments below.
       */
      get_metadata();
      break;
    case SUMP_SELF_TEST:
      /* ignored. */
      break;
    default:
      /* ignore any unrecognized bytes. */
      break;
  }
}

void sump_cmd(char c) {
  static sump_cmd_state read_state = STATE_CMD;
  static char last_cmd = 0;
  static uint8_t extended_cmd_arg[4];

  if(read_state == STATE_CMD) {
    last_cmd = c;
    sump_read_command(&read_state, c);
  } else if(read_state >= STATE_READ4 && read_state <= STATE_READ1) {
    extended_cmd_arg[read_state] = c;
    read_state = read_state + 1;
    if(read_state == STATE_EXTENDED_CMD) {
      extended_sump_command(last_cmd, extended_cmd_arg);
      read_state = STATE_CMD;
    }
  } else {
    write_uart_s("unknown state ");
    write_uart_u(read_state);
    write_uart_s("\n");
  }
}

void poll_sump() {
  if(run_gpio) {
    run_gpio = 0;
    if(divider < 11) {
      write_uart_s("using gpio_loop - IGNORING!\n");
      //do_gpio_loop(readCount, trigger, trigger_values);
    } else {
      do_gpio_dma(readCount, trigger, trigger_values, divider);
    }
    int status = CDC_Transmit_FS(gpio_buffer, readCount);
    if(status != USBD_OK) {
      write_uart_s("USB TX state: ");
      write_uart_u(status);
      write_uart_s("\n");
    }
  }
}
