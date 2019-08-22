/*
 * helper.c
 *
 *  Created on: May 19, 2012
 *      Author: mabl
 */

#include <string.h>

#include "flash.h"
#include "helper.h"

void linearFlashProgramStart(struct LinearFlashing* flash) {
  flash->pageBufferTainted = FALSE;
  flash->currentPage = 0;
}

int linearFlashProgramFinish(struct LinearFlashing* flash) {
  int err = 0;

  /* Write back last buffer if it is tainted */
  if (flash->pageBufferTainted) {
    err = flashPageWriteIfNeeded(flash->currentPage, flash->pageBuffer);
  }

  return err;
}

int linearFlashProgram(struct LinearFlashing* flash, ui32 address,
                          const flashdata_t* buffer, int length) {
  flashpage_t oldPage;
  int pagePosition;
  int processLen;
  bool writeback = FALSE;
  int err;

  /* Process all given words */
  while(length > 0) {
    oldPage = flash->currentPage;
    flash->currentPage = FLASH_PAGE_OF_ADDRESS(address);
    pagePosition = address - FLASH_ADDRESS_OF_PAGE(flash->currentPage);
    processLen = (FLASH_PAGE_SIZE - pagePosition);

    /* Read back new page if page as changed. */
    if(oldPage != flash->currentPage) {
      err = flashPageRead(flash->currentPage, flash->pageBuffer);

      /* Return if we get errors here. */
      if (err == CH_FAILED)
        return -1;

      flash->pageBufferTainted = FALSE;
    }

    /* Process no more bytes than remaining */
    if(processLen > length) {
      processLen = length;
    } else if (processLen <= length) {
      writeback = TRUE;
    }

    /* Copu buffer into page buffer and mark as tainted*/
    memcpy(&flash->pageBuffer[pagePosition / sizeof(flashdata_t)], buffer,
            processLen);
    flash->pageBufferTainted = TRUE;

    /* Decrease handled bytes from total length. */
    length -= processLen;

    /* Writeback buffer if needed */
    if (writeback) {
      err = flashPageWriteIfNeeded(flash->currentPage, flash->pageBuffer);

      /* Return if we get errors here. */
      if (err)
        return err;

      writeback = FALSE;
    }
  }

  return 0;
}

#if 0
void flashJumpApplication(ui32 address) {
  typedef void (*pFunction)(void);

  pFunction Jump_To_Application;

  /* variable that will be loaded with the start address of the application */
  vu32* JumpAddress;
  const vu32* ApplicationAddress = (vu32*) address;

  /* get jump address from application vector table */
  JumpAddress = (vu32*) ApplicationAddress[1];

  /* load this address into function pointer */
  Jump_To_Application = (pFunction) JumpAddress;

  /* reset all interrupts to default */
  chSysDisable();

  /* Clear pending interrupts just to be on the save side*/
  SCB_ICSR = ICSR_PENDSVCLR;

  /* Disable all interrupts */
  int i;
  for(i=0; i<8; i++)
    NVIC->ICER[i] = NVIC->IABR[i];


  /* set stack pointer as in application's vector table */
  __set_MSP((u32) (ApplicationAddress[0]));
  Jump_To_Application();
}
#endif

