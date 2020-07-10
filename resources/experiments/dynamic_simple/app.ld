MEMORY
{
  ram (rwx) : ORIGIN = 0x20008000, LENGTH = 4K /* +32k	, size 4k*/
}

_estack = ORIGIN(ram)+LENGTH(ram)-0x100;

SECTIONS
  {
    .text : {
      _vectors = .;
      *(.isr_vectors)      /* Vector table */
      *(.text*)        /* Program code */
      *(.rodata*)      /* Read only data */

      . = ALIGN(4);

      _etext = .;
      _sidata = _etext;
    } >ram


    .data : {
      _sdata = . ;
      *(.data)        /* Data memory */
      _edata = .;
    } >ram

  .bss : {
    _sbss = .;
    *(.bss)         /* Zero-filled run time allocate data memory */
    _ebss = .;
    } >ram
  }  

