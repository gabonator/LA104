MEMORY
{
  rom (rx)  : ORIGIN = 0x0801ed00, LENGTH = 60K
  ram (rwx) : ORIGIN = 0x20005000, LENGTH = 16K
  null (rwx): ORIGIN = 0x00001000, LENGTH = 4K
}

/* _estack = ORIGIN(ram)+LENGTH(ram)-0x100; */

SECTIONS
  {
    .text : {
      *(.entry)
      *(.text)
      *(.text.*)        /* Program code */
      *(.rodata*)      /* Read only data */
    } >ram

    .data : {
      *(.data*)        /* Data memory */
    } >ram 

  .bss : {
    *(.bss*)         /* Zero-filled run time allocate data memory */
    } >ram

  .rel.plt : { *(.rel.plt)  } > ram
  .plt : { *(.plt) } > ram
  .got : { *(.got.plt) *(.got) } > ram 
  .dynsym : { *(.dynsym) } > null
  .dynstr : { *(.dynstr) } > null
  .dynamic : { *(.dynstr) } > null
  .interp : { *(.dynstr) } > null
  .hash : { *(.dynstr) } > null
  }  
