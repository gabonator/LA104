/*
08008000..08014c60
20000000..20004534

08008000..08030000 allrom
20000000..2000a000 allram
*/


MEMORY
{
/*
  rom (rx) : ORIGIN = 0x08015000, LENGTH = 20K
  ram (rwx) : ORIGIN = 0x20005000, LENGTH = 6K
  null (rwx): ORIGIN = 0x00001000, LENGTH = 4K
*/
  rom (rx) :   ORIGIN = 0x08020000, LENGTH = 64K
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

