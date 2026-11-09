/* DS203 Firmware Updater - Linker Script
 *
 * Placed at APP4 slot (0x08024000) to stay clear of the firmware
 * being flashed at APP1 (0x0800C000-0x08013FFF).
 *
 * RAM starts at 0x20005000 to avoid SYS BIOS RAM (0x20000000-0x20002FFF)
 * and os_host RAM (0x20003000-0x20004FFF).
 */
ENTRY(_start)

MEMORY
{
  rom  (rx)  : ORIGIN = 0x08024000, LENGTH = 32K
  ram  (rwx) : ORIGIN = 0x20005000, LENGTH = 28K
  null (rwx) : ORIGIN = 0x00001000, LENGTH = 4K
}

SECTIONS
{
  .text : {
    *(.entry)
    *(.text*)
    *(.rodata*)
  } > rom

  .data : {
    *(.data*)
  } > ram AT > rom

  .bss : {
    *(.bss*)
    *(COMMON)
  } > ram

  .rel.plt : { *(.rel.plt)  } > rom
  .plt     : { *(.plt)      } > rom
  .got     : { *(.got.plt) *(.got) } > ram
  .dynsym  : { *(.dynsym)   } > null
  .dynstr  : { *(.dynstr)   } > null
}
