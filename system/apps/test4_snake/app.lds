MEMORY
{
  rom (rx) : ORIGIN = 0x08020000, LENGTH = 24K
  ram (rwx) : ORIGIN = 0x20007000, LENGTH = 4K
  null (rwx): ORIGIN = 0x00001000, LENGTH = 4K
}

/* _estack = ORIGIN(ram)+LENGTH(ram)-0x100; */

SECTIONS
  {
    .text : {
      *(.entry)
      *(.text)
      *(.text*)        /* Program code */
      *(.rodata*)      /* Read only data */
    } >rom

    .data : {
      *(.data)        /* Data memory */
      *(.data*)
      *(.init_array)
    } >ram

  .bss : {
    *(.bss)         /* Zero-filled run time allocate data memory */
    *(.bss*)
    } >ram

  .rel.plt : { *(.rel.plt)  } > rom
  .plt : { *(.plt) } > rom
  .got : { *(.got.plt) *(.got) } > ram /* relocation fixed by memory write! */
  .dynsym : { *(.dynsym) } > null
  .dynstr : { *(.dynstr) } > null

  }  

