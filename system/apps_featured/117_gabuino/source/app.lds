MEMORY
{
/*  rom (rx)  : ORIGIN = 0x0801ed00, LENGTH = 60K*/
  rom (rx) : ORIGIN = 0x08064100, LENGTH = 80K

  ram (rwx) : ORIGIN = 0x20004780, LENGTH = 4K
  ramuser (rwx) : ORIGIN = 0x20005000, LENGTH = 8K
  null (rwx): ORIGIN = 0x00001000, LENGTH = 8K
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
      *(.data*)        /* Data memory */
    } >ram

    .datauser : {
       KEEP(*(.usersection))
    } > ramuser

  .bss : {
    *(.bss*)         /* Zero-filled run time allocate data memory */
    } >ram

  .rel.plt : { *(.rel.plt)  } > rom
  .plt : { *(.plt) 
      _addressRomEnd = .;
} > rom
  .got : { *(.got.plt) *(.got) 
      _addressRamEnd = .;
} > ram /* relocation fixed by memory write! */
  .dynsym : { *(.dynsym) } > null
  .dynstr : { *(.dynstr) } > null

  }  

