MEMORY
{
  rom (rx) : ORIGIN = 0x08013e00, LENGTH = 20K
  ram (rwx) : ORIGIN = 0x20004800, LENGTH = 16K
  null (rwx): ORIGIN = 0x00001000, LENGTH = 4K
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
    } >ram AT >rom

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

