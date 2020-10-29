MEMORY
{
  null (rwx): ORIGIN = 0x00001000, LENGTH = 4K
  ram (rwx)  : ORIGIN = 0x20005000, LENGTH = 28K
  rom (rx)   : ORIGIN = 0x08024000, LENGTH = 32K
  extra (rx) : ORIGIN = 0x08064000, LENGTH = 32K
}

SECTIONS
  {
/*
    .rodata : {
        *(.rodata)
        *(.extra)
    } >extra
 */
    .text : {
      *(.entry)
      *(.text*)        /* Program code */
      *(.rodata*)      /* Read only data */
    } >rom

    .data : {
      *(.data*)        /* Data memory */
      *(.dynamic)
    } >ram AT >rom

  .bss : {
    *(.bss*)         /* Zero-filled run time allocate data memory */
    } >ram

  .rel.plt : { *(.rel.plt)  } > rom
  .plt : { *(.plt) } > rom
  .got : { *(.got.plt) *(.got) } > ram /* relocation fixed by memory write! */
  .dynsym : { *(.dynsym) } > null
  .dynstr : { *(.dynstr) } > null
  .interp : { *(.interp) } > null
  .hash : { *(.interp) } > null

  }  

