/*
Placed at end of RAM/ROM
08008000..08030000 allrom
20000000..2000a000 allram
*/

MEMORY
{
  rom (rx) :   ORIGIN = 0x0802F000, LENGTH = 4K
  ram (rwx) :  ORIGIN = 0x20009F00, LENGTH = 256
  null (rwx) : ORIGIN = 0xf0000000, LENGTH = 4K

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
