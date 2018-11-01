#ifndef FLASHCONFIG_H_
#define FLASHCONFIG_H_

/**
 * @brief Page size of flash in kilobytes
 */
#if !defined(FLASH_PAGE_SIZE) || defined(__DOXYGEN__)
#define FLASH_PAGE_SIZE         2048
#endif

/**
 * @brief Number of flash pages
 */
#if !defined(FLASH_PAGE_NUMBER) || defined(__DOXYGEN__)
#define FLASH_PAGE_NUMBER       128
#endif

/**
 * @brief Start address of user application
 */
#if !defined(FLASH_USER_BASE) || defined(__DOXYGEN__)
#define FLASH_USER_BASE         ((ui32)0x08005000)
#endif

#if !defined(FLASH_BASE) || defined(__DOXYGEN__)
#define FLASH_BASE FLASH_USER_BASE
#endif

#endif /* FLASHCONFIG_H_ */
