MEMORY
{
  rom (rx) : ORIGIN = 0x08020000, LENGTH = 63K
  ram (rwx) : ORIGIN = 0x20004200, LENGTH = 20K
  null (rwx): ORIGIN = 0x00001000, LENGTH = 4K
}

SECTIONS
  {
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

  }  

