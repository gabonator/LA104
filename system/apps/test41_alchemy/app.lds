MEMORY
{
  rom (rx)  : ORIGIN = 0x0801ee00, LENGTH = 63K
  ram (rwx) : ORIGIN = 0x20004400, LENGTH = 10K
  null (rwx): ORIGIN = 0x00001000, LENGTH = 4K
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

