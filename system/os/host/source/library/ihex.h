#ifndef INTEL_HEX_H
#define INTEL_HEX_H
/**
 * \file ihex.h
 * \brief Low-level utility functions to create, read, write, and print Intel HEX8 binary records.
 * \author Vanya A. Sergeev <vsergeev@gmail.com>
 * \date February 2011
 * \version 1.0.5
 */

#ifdef _WIN32
ui32 __strtol(char*str, char**p, int nBase);

ui32 _strtol(char*str, char**p, int nBase)
{ 
	_ASSERT( strtol(str, p, nBase) == __strtol(str, p, nBase) );
	return strtol(str, p, nBase);
}
ui32 __strtol(char*str, char**p, int nBase)
#else
ui32 _strtol(char*str, char**p, int nBase)
#endif
{
	ui32 nVal = 0;
	char ch;
	while ( (ch = *str++) != 0 )
	{
		nVal <<= 4;
		if ( ch >= '0' && ch <= '9' )
			nVal |= ch - '0';	
		if ( ch >= 'a' && ch <= 'f' )
			nVal |= ch - 'a' + 10;	
		if ( ch >= 'A' && ch <= 'F' )
			nVal |= ch - 'A' + 10;	
	}
	return nVal;
}

/* General definition of the Intel HEX8 specification */
enum _IHexDefinitions {
	/* 768 should be plenty of space to read in a Intel HEX8 record */
	IHEX_RECORD_BUFF_SIZE = 64,
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

uint8_t Checksum_IHexRecord(const IHexRecord *ihexRecord);

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
int New_IHexRecord(int type, uint16_t address, const uint8_t *data, int dataLen, IHexRecord *ihexRecord)
{
  /* Data length size check, assertion of ihexRecord pointer */
  if (dataLen < 0 || dataLen > IHEX_MAX_DATA_LEN / 2 || ihexRecord == NULL)
    return IHEX_ERROR_INVALID_ARGUMENTS;

  ihexRecord->type = type;
  ihexRecord->address = address;
  memcpy(ihexRecord->data, data, dataLen);
  ihexRecord->dataLen = dataLen;
  ihexRecord->checksum = Checksum_IHexRecord(ihexRecord);

  return IHEX_OK;
}

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
int Read_IHexRecord(IHexRecord *ihexRecord, CBufferedReader2 &in)
{
  char recordBuff[IHEX_RECORD_BUFF_SIZE];
  /* A temporary buffer to hold ASCII hex encoded data, set to the maximum length we would ever need */
  char hexBuff[IHEX_ADDRESS_LEN + 1];
  int dataCount, i;

  /* Check our record pointer and FIL pointer */
  if (ihexRecord == NULL )
    return IHEX_ERROR_INVALID_ARGUMENTS;

  in >> recordBuff;
#if 0
  if (f_gets(recordBuff, IHEX_RECORD_BUFF_SIZE, in) == NULL) {
    /* In case we hit EOF, don't report a FIL error */
    if (f_eof(in) != 0)
      return IHEX_ERROR_EOF;
    else
      return IHEX_ERROR_FILE;
  }
  /* Null-terminate the string at the first sign of a \r or \n */
  for (i = 0; i < (int) strlen(recordBuff); i++) {
    if (recordBuff[i] == '\r' || recordBuff[i] == '\n') {
      recordBuff[i] = 0;
      break;
    }
  }
#endif

  /* Check if we hit a newline */
  if (strlen(recordBuff) == 0)
    return IHEX_ERROR_NEWLINE;

  /* Size check for start code, count, addess, and type fields */
  if (strlen(recordBuff)
      < (unsigned int) (1 + IHEX_COUNT_LEN + IHEX_ADDRESS_LEN + IHEX_TYPE_LEN))
    return IHEX_ERROR_INVALID_RECORD;

  /* Check the for colon start code */
  if (recordBuff[IHEX_START_CODE_OFFSET] != IHEX_START_CODE)
    return IHEX_ERROR_INVALID_RECORD;

  /* Copy the ASCII hex encoding of the count field into hexBuff, convert it to a usable integer */
  strncpy(hexBuff, recordBuff + IHEX_COUNT_OFFSET, IHEX_COUNT_LEN);
  hexBuff[IHEX_COUNT_LEN] = 0;
  dataCount = _strtol(hexBuff, (char **) NULL, 16);

  /* Copy the ASCII hex encoding of the address field into hexBuff, convert it to a usable integer */
  strncpy(hexBuff, recordBuff + IHEX_ADDRESS_OFFSET, IHEX_ADDRESS_LEN);
  hexBuff[IHEX_ADDRESS_LEN] = 0;
  ihexRecord->address = (uint16_t) _strtol(hexBuff, (char **) NULL, 16);

  /* Copy the ASCII hex encoding of the address field into hexBuff, convert it to a usable integer */
  strncpy(hexBuff, recordBuff + IHEX_TYPE_OFFSET, IHEX_TYPE_LEN);
  hexBuff[IHEX_TYPE_LEN] = 0;
  ihexRecord->type = _strtol(hexBuff, (char **) NULL, 16);

  /* Size check for start code, count, address, type, data and checksum fields */
  if (strlen(recordBuff)
      < (unsigned int) (1 + IHEX_COUNT_LEN + IHEX_ADDRESS_LEN + IHEX_TYPE_LEN
          + dataCount * 2 + IHEX_CHECKSUM_LEN))
    return IHEX_ERROR_INVALID_RECORD;

  /* Loop through each ASCII hex byte of the data field, pull it out into hexBuff,
   * convert it and store the result in the data buffer of the Intel HEX8 record */
  for (i = 0; i < dataCount; i++) {
    /* Times two i because every byte is represented by two ASCII hex characters */
    strncpy(hexBuff, recordBuff + IHEX_DATA_OFFSET + 2 * i,
        IHEX_ASCII_HEX_BYTE_LEN);
    hexBuff[IHEX_ASCII_HEX_BYTE_LEN] = 0;
    ihexRecord->data[i] = (uint8_t) _strtol(hexBuff, (char **) NULL, 16);
  }
  ihexRecord->dataLen = dataCount;

  /* Copy the ASCII hex encoding of the checksum field into hexBuff, convert it to a usable integer */
  strncpy(hexBuff, recordBuff + IHEX_DATA_OFFSET + dataCount * 2,
      IHEX_CHECKSUM_LEN);
  hexBuff[IHEX_CHECKSUM_LEN] = 0;
  ihexRecord->checksum = (uint8_t) _strtol(hexBuff, (char **) NULL, 16);

  if (ihexRecord->checksum != Checksum_IHexRecord(ihexRecord))
    return IHEX_ERROR_INVALID_RECORD;

  return IHEX_OK;
}


/**
 * Calculates the checksum of an Intel HEX8 IHexRecord structure.
 * See the Intel HEX8 specifications for more details on the checksum calculation.
 * \param ihexRecord A pointer to the Intel HEX8 record structure.
 * \return The 8-bit checksum.
*/
uint8_t Checksum_IHexRecord(const IHexRecord *ihexRecord)
{
  uint8_t checksum;
  int i;

  /* Add the data count, type, address, and data bytes together */
  checksum = ihexRecord->dataLen;
  checksum += ihexRecord->type;
  checksum += (uint8_t) ihexRecord->address;
  checksum += (uint8_t) ((ihexRecord->address & 0xFF00) >> 8);
  for (i = 0; i < ihexRecord->dataLen; i++)
    checksum += ihexRecord->data[i];

  /* Two's complement on checksum */
  checksum = ~checksum + 1;

  return checksum;
}

#endif
