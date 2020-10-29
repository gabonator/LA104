MEMORY
{
  rom (rx)  : ORIGIN = 0x0801ee00, LENGTH = 63K
  ram (rwx) : ORIGIN = 0x20004800, LENGTH = 16K
  null (rwx): ORIGIN = 0x00001000, LENGTH = 4K
}

/* _estack = ORIGIN(ram)+LENGTH(ram)-0x100; */

SECTIONS
  {
    .text : {
      *(.entry)
      *(.text*)        /* Program code */
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
  .got : { *(.got.plt) *(.got) } > ram /* relocation fixed by memory write! */
  .dynsym : { *(.dynsym) } > null
  .dynstr : { *(.dynstr) } > null

  }  

