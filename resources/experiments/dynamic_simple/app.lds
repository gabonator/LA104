/* Define memory regions. */
MEMORY
{
/*  rom (rx) : ORIGIN = 0x08014000, LENGTH = 96K */
  rom (rx) : ORIGIN = 0x08014000, LENGTH = 96K 
/*  ram (rwx) : ORIGIN = 0x20003000, LENGTH = 36K*/
/* ram (rwx) : ORIGIN = 0x20004000, LENGTH = 4K */
	ram (rwx) : ORIGIN = 0x20005000, LENGTH = 2K
}

_estack = ORIGIN(ram)+LENGTH(ram)-0x100;

/* Linker script for Olimex STM32-H103 eval board.
 *
 * Version: 0.1
 *
 *  Copyright 2007 CodeSourcery.
 *
 * The authors hereby grant permission to use, copy, modify, distribute,
 * and license this software and its documentation for any purpose, provided
 * that existing copyright notices are retained in all copies and that this
 * notice is included verbatim in any distributions. No written agreement,
 * license, or royalty fee is required for any of the authorized uses.
 * Modifications to this software may be copyrighted by their authors
 * and need not follow the licensing terms described here, provided that
 * the new terms are clearly indicated on the first page of each file where
 * they apply.
 *
 * Modified by Olimex
 * 2009-04-07: Modified by Piotr Esden-Tempski <piotr@esden.net>
 */

SECTIONS
  {
    .text : {
/*      _vectors = .;*/
      *(.identify)
      *(.text*)        /* Program code */
      *(.rodata*)      /* Read only data */

      _etext = .;
      _sidata = _etext;
    } >ram

/*    .  = 0x20000000;  */ /* From 0x20000000 */      

    .data : {
      _sdata = . ;
      *(.data)        /* Data memory */
/*      _edata = .;*/
    } >ram /*AT > rom*/

  .bss : {
    _sbss = .;
    *(.bss)         /* Zero-filled run time allocate data memory */
    _ebss = .;
    } >ram /*AT > rom*/

.rel.plt : { *(.rel.plt)  } >ram
.plt : { *(.plt) } >ram
 .got            : { *(.got.plt) *(.got) } > ram

  }  
/*========== end of file ==========*/
