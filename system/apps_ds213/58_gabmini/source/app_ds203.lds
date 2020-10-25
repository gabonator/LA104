MEMORY
{
  null (rwx): ORIGIN = 0x00001000, LENGTH = 4K
  rom (rx) : ORIGIN = 0x08012c00, LENGTH = 40K
  ram (rwx) : ORIGIN = 0x20005000, LENGTH = 28K
}

/*
failed 648 bytes at 0801f648
                    0801f7f4
                    0802af30
                    08029f30
                    0801f050
*/
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
      *(.dynamic)
    } >ram

  .bss : {
    *(.bss*)         /* Zero-filled run time allocate data memory */
    _addressRamEnd = .;
    } >ram

  .rel.plt : { *(.rel.plt)  } > rom
  .plt : { *(.plt) } > rom
  .got : { *(.got.plt) *(.got) } > ram /* relocation fixed by memory write! */
  .dynsym : { *(.dynsym) } > null
  .dynstr : { *(.dynstr) } > null

  }  

