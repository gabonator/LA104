MEMORY
{
  rom (rx) :   ORIGIN = 0x08020400, LENGTH = 64K
  ram (rwx) :  ORIGIN = 0x20005000, LENGTH = 6K
  null (rwx) : ORIGIN = 0xf0000000, LENGTH = 4K
}

SECTIONS
{
    .text : {
      *(.entry)
      *(.text*)
      *(.rodata*)
    } >rom
    .data : {
      *(.data*)
    } >ram
    .bss : {
      *(.bss*)
    } >ram

    .rel.plt : { *(.rel.plt) } > rom
    .plt : { *(.plt) } > rom
    .got : { *(.got.plt) *(.got) } > ram
    .dynsym : { *(.dynsym) } > null
    .dynstr : { *(.dynstr) } > null
}
