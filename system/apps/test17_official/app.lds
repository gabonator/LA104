MEMORY
{
  rom (rx) : ORIGIN = 0x08040000, LENGTH = 256K
  ram (rwx) : ORIGIN = 0x20002700, LENGTH = 30K
  null (rwx): ORIGIN = 0x00001000, LENGTH = 8K
}

/* _estack = ORIGIN(ram)+LENGTH(ram)-0x100; */

SECTIONS
  {

  .bios 0x08000100 (NOLOAD) : {
    *(.biosfunc) 
  }

    .text : {
      *(.entry)
      *(.text*)        /* Program code */
      *(.rodata*)      /* Read only data */
    } >rom

    .data : {
      *(.data*)        /* Data memory */
/*      *(.dynamic*) */
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

