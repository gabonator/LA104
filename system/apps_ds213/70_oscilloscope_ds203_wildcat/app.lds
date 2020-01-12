MEMORY
{
  rom (rx) : ORIGIN = 0x08040000, LENGTH = 256K
  null (rwx): ORIGIN = 0x00001000, LENGTH = 8K

  ram (rwx) : ORIGIN = 0x20004300, LENGTH = 42K
/*  ram (rwx) : ORIGIN = 0x20003000, LENGTH = 42K*/
}


SECTIONS
  {
    .bios 0x08004200 (NOLOAD) : {
      BIOS.o
    }

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

