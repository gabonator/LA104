#ifndef INTEL_HEX_H
#define INTEL_HEX_H
/**
 * \file ihex.h
 * \brief Low-level utility functions to create, read, write, and print Intel HEX8 binary records.
 * \author Vanya A. Sergeev <vsergeev@gmail.com>
 * \date February 2011
 * \version 1.0.5
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "ff.h"

/* General definition of the Intel HEX8 specification */
enum _IHexDefinitions {
	/* 768 should be plenty of space to read in a Intel HEX8 record */
	IHEX_RECORD_BUFF_SIZE = 768,
	/* Offsets and lengths of various fields in an Intel HEX8 record */
	IHEX_COUNT_OFFSET = 1,
	IHEX_COUNT_LEN = 2,
	IHEX_ADDRESS_OFFSET = 3,
	IHEX_ADDRESS_LEN = 4,
	IHEX_TYPE_OFFSET = 7,
	IHEX_TYPE_LEN = 2,
	IHEX_DATA_OFFSET = 9,
	IHEX_CHECKSUM_LEN = 2,
	IHEX_MAX_DATA_LEN = 512,
	/* Ascii hex encoded length of a single byte */
	IHEX_ASCII_HEX_BYTE_LEN = 2,
	/* Start code offset and value */
	IHEX_START_CODE_OFFSET = 0,
	IHEX_START_CODE = ':',
};

/**
 * All possible error codes the Intel HEX8 record utility functions may return.
 */
enum IHexErrors {
	IHEX_OK = 0, 				/**< Error code for success or no error. */
	IHEX_ERROR_FILE = -1, 			/**< Error code for error while reading from or writing to a file. You may check errno for the exact error if this error code is encountered. */
	IHEX_ERROR_EOF = -2, 			/**< Error code for encountering end-of-file when reading from a file. */
	IHEX_ERROR_INVALID_RECORD = -3, 	/**< Error code for error if an invalid record was read. */
	IHEX_ERROR_INVALID_ARGUMENTS = -4, 	/**< Error code for error from invalid arguments passed to function. */
	IHEX_ERROR_NEWLINE = -5, 		/**< Error code for encountering a newline with no record when reading from a file. */
};

/**
 * Intel HEX8 Record Types 00-05
 */
enum IHexRecordTypes {
	IHEX_TYPE_00 = 0, /**< Data Record */
	IHEX_TYPE_01, /**< End of FIL Record */
	IHEX_TYPE_02, /**< Extended Segment Address Record */
	IHEX_TYPE_03, /**< Start Segment Address Record */
	IHEX_TYPE_04, /**< Extended Linear Address Record */
	IHEX_TYPE_05, /**< Start Linear Address Record */
};

/**
 * Structure to hold the fields of an Intel HEX8 record.
 */
typedef struct {
	uint16_t address; 			/**< The 16-bit address field. */
	uint8_t data[IHEX_MAX_DATA_LEN/2]; 	/**< The 8-bit array data field, which has a maximum size of 256 bytes. */
	int dataLen; 				/**< The number of bytes of data stored in this record. */
	int type; 				/**< The Intel HEX8 record type of this record. */
	uint8_t checksum; 			/**< The checksum of this record. */
} IHexRecord;

/**
 * Sets all of the record fields of an Intel HEX8 record structure.
 * \param type The Intel HEX8 record type (integer value of 0 through 5).
 * \param address The 16-bit address of the data.
 * \param data A point to the 8-bit array of data.
 * \param dataLen The size of the 8-bit data array.
 * \param ihexRecord A pointer to the target Intel HEX8 record structure where these fields will be set.
 * \return IHEX_OK on success, otherwise one of the IHEX_ERROR_ error codes.
 * \retval IHEX_OK on success.
 * \retval IHEX_ERROR_INVALID_ARGUMENTS if the record pointer is NULL, or if the length of the 8-bit data array is out of range (less than zero or greater than the maximum data length allowed by record specifications, see IHexRecord.data).
*/
int New_IHexRecord(int type, uint16_t address, const uint8_t *data, int dataLen, IHexRecord *ihexRecord);

/**
 * Reads an Intel HEX8 record from an opened file.
 * \param ihexRecord A pointer to the Intel HEX8 record structure that will store the read record.
 * \param in A FIL pointer to an opened FIL that can be read.
 * \return IHEX_OK on success, otherwise one of the IHEX_ERROR_ error codes.
 * \retval IHEX_OK on success.
 * \retval IHEX_ERROR_INVALID_ARGUMENTS if the record pointer or FIL pointer is NULL.
 * \retval IHEX_ERROR_EOF if end-of-file has been reached.
 * \retval IHEX_ERROR_FILE if a FIL reading error has occured.
 * \retval IHEX_INVALID_RECORD if the record read is invalid (record did not match specifications or record checksum was invalid).
*/
int Read_IHexRecord(IHexRecord *ihexRecord, FIL *in);

/**
 * Calculates the checksum of an Intel HEX8 IHexRecord structure.
 * See the Intel HEX8 specifications for more details on the checksum calculation.
 * \param ihexRecord A pointer to the Intel HEX8 record structure.
 * \return The 8-bit checksum.
*/
uint8_t Checksum_IHexRecord(const IHexRecord *ihexRecord);

#endif
