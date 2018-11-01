/*
 * flash.c
 *
 *  Created on: May 19, 2012
 *      Author: mabl
 */

#ifndef FLASH_C_
#define FLASH_C_

#include "flashconfig.h"

/**
 * @brief Flash upper bound.
 */
#define FLASH_TOP       (FLASH_BASE + FLASH_PAGE_SIZE*FLASH_PAGE_NUMBER - 1)

/**
 * @brief Get page from flash address.
 */
#define FLASH_PAGE_OF_ADDRESS(address)                                       \
                              ((address - FLASH_BASE) / FLASH_PAGE_SIZE)

/**
 * @brief Get page from flash address.
 */
#define FLASH_ADDRESS_OF_PAGE(page)                                       \
                              (FLASH_BASE + page * FLASH_PAGE_SIZE)

/**
 * @brief Check if the given address is within the bootloader.
 */
#define FLASH_IS_ADDRESS_BOOTLOADER(address)                                 \
                        (address >= FLASH_BASE && address < FLASH_USER_BASE)

/**
 * @brief Check if the given address is within user flash region.
 */
#define FLASH_IS_ADDRESS_USERSPACE(address)                                  \
                        (address >= FLASH_USER_BASE && address <= FLASH_TOP)


/* Error codes */

/** @brief  Flash operation successful */
#define FLASH_RETURN_SUCCESS     CH_SUCCESS

/** @brief  Flash operation error because of denied access, corrupted memory.*/
#define FLASH_RETURN_NO_PERMISSION      -1

/** @brief  Flash operation error because of bad flash, corrupted memory     */
#define FLASH_RETURN_BADFLASH     -11

/** @brief  Test if error corrupted flash                                    */
#define FLASH_CORRUPTED_FLASH_ERROR(error)  (error <= -10)


#ifdef __cplusplus
extern "C" {
#endif

typedef unsigned int flashpage_t;
typedef ui16 flashdata_t;

/**
 * @brief   Erase a flash @p page belonging to the user application.
 * @details The page is checked for errors after erase.
 * @note    The page is deleted regardless of its current state.
 *
 * @param page  Page which is going to be deleted.
 * @return FLASH_RETURN_SUCCESS         No error erasing the page.
 * @return FLASH_RETURN_BADFLASH        Flash cell error.
 * @return FLASH_RETURN_NO_PERMISSION   Access denied.
 */
int flashPageErase(flashpage_t page);

/**
 * @brief   Check if the @p page is already erased.
 * @param   page Page to check.
 * @return TRUE  Page is already erased.
 * @return FALSE Page is not erased.
 */
bool flashPageCheckErased(flashpage_t page);

/**
 * @brief Read @p page into @p buffer.
 * @param[in]  page     Page to read.
 * @param[out] buffer   Buffer with FLASH_PAGE_SIZE bytes space.
 * @return CH_SUCCESS   Page successfully read.
 * @return CH_FAILED    Page read failed.
 */
bool flashPageRead(flashpage_t page, flashdata_t* buffer);

/**
 * @brief Compare a @p buffer with a @p page in memory.
 * @param page
 * @param buffer
 * @return  0   Page is identical. No write is needed.
 * @return  1   Page is not identical, but no page erase is needed to write.
 * @return  2   Page is not identical, page erase is needed.
 * @return  -1  Compare failed.
 */
int flashPageCompare(flashpage_t page, const flashdata_t* buffer);

/**
 * @brief   Write @p buffer to flash memory @p page.
 * @details You have to check yourself if you need to erase the page before
 *          writing with @p flashPageCompare.
 * @param page
 * @param buffer
 * @return FLASH_RETURN_SUCCESS         No error deleting the page.
 * @return FLASH_RETURN_BADFLASH        Flash cell error.
 * @return FLASH_RETURN_NO_PERMISSION   Access denied.
 */
int flashPageWrite(flashpage_t page, const flashdata_t* buffer);

/**
 * @brief   Write @p buffer to flash memory @p page and perform needed actions.
 * @details This function only erases the page and writes if needed.
 * @param page
 * @param buffer
 * @return FLASH_RETURN_SUCCESS         No error deleting the page.
 * @return FLASH_RETURN_BADFLASH        Flash cell error.
 * @return FLASH_RETURN_NO_PERMISSION   Access denied.
 */
int flashPageWriteIfNeeded(flashpage_t page, const flashdata_t* buffer);



#ifdef __cplusplus
}
#endif

#endif /* FLASH_C_ */
