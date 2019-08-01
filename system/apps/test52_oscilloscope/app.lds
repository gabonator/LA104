MEMORY
{
  rom (rx) :   ORIGIN = 0x08020000, LENGTH = 64K
  ram (rwx) :  ORIGIN = 0x20005000, LENGTH = 12K
  null (rwx) : ORIGIN = 0xf0000000, LENGTH = 10K
}

/* _estack = ORIGIN(ram)+LENGTH(ram)-0x100; */

SECTIONS
  {
  .bios 0x08000100 (NOLOAD) : {
    *(.biosfunc) 
  }

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
  .ARM.exidx : { *(.ARM.exidx) } > null
  }  

