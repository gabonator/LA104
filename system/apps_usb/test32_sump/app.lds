/*
08008000..08014c60
20000000..20004534

08008000..08030000 allrom
20000000..2000a000 allram


0x08008000 + 0x00cc08
0x20000278 + 0x0043ec
  [ 2] .text             PROGBITS        08008000 008000 00cc08 00  AX  0   0  4
  [ 3] .data             PROGBITS        20000000 020000 00026d 00  WA  0   0  4
  [ 4] .init_array       INIT_ARRAY      20000270 020270 000008 04  WA  0   0  4
  [ 5] .bss              NOBITS          20000278 020278 0043ec 00  WA  0   0  4

*/


MEMORY
{
/*
  rom (rx) : ORIGIN = 0x08015000, LENGTH = 20K
  ram (rwx) : ORIGIN = 0x20005000, LENGTH = 6K
  null (rwx): ORIGIN = 0x00001000, LENGTH = 4K
*/
  rom (rx) :   ORIGIN = 0x08020000, LENGTH = 20K
  ram (rwx) :  ORIGIN = 0x20005000, LENGTH = 10K
  null (rwx) : ORIGIN = 0xf0000000, LENGTH = 4K

}

/* _estack = ORIGIN(ram)+LENGTH(ram)-0x100; */

SECTIONS
  {
    .text : {
      *(.entry)
      *(.text*)        /* Program code */
      *(.rodata*)      /* Read only data */
    } >rom
    .data : {
      *(.data*)        /* Data memory */
    } >ram
  .bss : {
    *(.bss*)         /* Zero-filled run time allocate data memory */
    } >ram

  .rel.plt : { *(.rel.plt)  } > rom
  .plt : { *(.plt) } > rom
  .got : { *(.got.plt) *(.got) } > ram /* relocation fixed by memory write! */
  .dynsym : { *(.dynsym) } > null
  .dynstr : { *(.dynstr) } > null

  }  

