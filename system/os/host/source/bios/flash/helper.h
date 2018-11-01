/*
 * helper.h
 *
 *  Created on: May 19, 2012
 *      Author: mabl
 */

#ifndef HELPER_H_
#define HELPER_H_

struct LinearFlashing {
  flashdata_t pageBuffer[FLASH_PAGE_SIZE / sizeof(flashdata_t)];
  bool pageBufferTainted;
  flashpage_t currentPage;
};

/**
 * @brief   Start linear flash programming,
 * @param[in] flash Pointer to a @p LinearFlashing object.
 */
void linearFlashProgramStart(struct LinearFlashing* flash);

/**
 * @brief   Finish up one linear flash programming session.
 * @details This call will write back the buffered data of the last page in
 *          the @p  LinearFlashing object.
 * @param flash Pointer to a @p LinearFlashing object.
 * @return
 */
int linearFlashProgramFinish(struct LinearFlashing* flash);

/**
 * @brief Write data from a buffer at a given address in flash.
 *
 * @details The flash page is read back and saved in the supplied
 *          @p LinearFlashing object. Each time the buffer exceeds a page
 *          border the old buffer in the @p LinearFlashing is written back to
 *          ROM and the next page is read.
 *
 *          The routine is trying to minimize write access. Preferably you
 *          should therefore not "hop" between pages.
 *
 * @param flash     Pointer to a @p LinearFlashing object.
 * @param address   Start address of the buffer
 * @param buffer    Buffer holding the data which is going to be written
 *                  starting at @p address.
 * @param length    Length of the data.
 * @return
 */
int linearFlashProgram(struct LinearFlashing* flash, ui32 address,
                          const flashdata_t* buffer, int length);



/**
 * @brief Jump to application located in flash.
 * @param address
 *
 * @author  Code stolen from "matis"
 * @link http://forum.chibios.org/phpbb/viewtopic.php?f=2&t=338
 */
void flashJumpApplication(ui32 address);


#endif /* HELPER_H_ */
