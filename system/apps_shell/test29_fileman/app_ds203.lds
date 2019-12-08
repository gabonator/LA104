MEMORY
{
  rom (rx) : ORIGIN = 0x08019000, LENGTH = 16K
  ram (rwx) : ORIGIN = 0x20005200, LENGTH = 4K
  null (rwx): ORIGIN = 0x00001000, LENGTH = 4K
/*
20004bcd 00000000 B _ebss
0801688c 00000000 T _etext

*/
}

/* _estack = ORIGIN(ram)+LENGTH(ram)-0x100; */

SECTIONS
  {
    .text : {
      _addressRomBegin = .;
      *(.entry)
      *(.text*)        /* Program code */
      *(.rodata*)      /* Read only data */
      _addressRomEnd = .;
    } >rom

    .data : {
      _addressRamBegin = .;
      *(.data)        /* Data memory */
    } >ram

  .bss : {
    *(.bss)         /* Zero-filled run time allocate data memory */
      _addressRamEnd = .;
    } >ram

  .rel.plt : { *(.rel.plt)  } > rom
  .plt : { *(.plt) } > rom
  .got : { *(.got.plt) *(.got) } > ram /* relocation fixed by memory write! */
  .dynsym : { *(.dynsym) } > null
  .dynstr : { *(.dynstr) } > null

  }  

