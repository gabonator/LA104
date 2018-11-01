/*
 *  ihex.c
 *  Utility functions to create, read, write, and print Intel HEX8 binary records.
 *
 *  Written by Vanya A. Sergeev <vsergeev@gmail.com>
 *  Version 1.0.5 - February 2011
 *
 */

#include "ihex.h"

/* Initializes a new IHexRecord structure that the paramater ihexRecord points to with the passed
 * record type, 16-bit integer address, 8-bit data array, and size of 8-bit data array. */
int New_IHexRecord(int type, uint16_t address, const uint8_t *data, int dataLen,
    IHexRecord *ihexRecord) {
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

/* Utility function to read an Intel HEX8 record from a FIL */
int Read_IHexRecord(IHexRecord *ihexRecord, FIL *in) {
  char recordBuff[IHEX_RECORD_BUFF_SIZE];
  /* A temporary buffer to hold ASCII hex encoded data, set to the maximum length we would ever need */
  char hexBuff[IHEX_ADDRESS_LEN + 1];
  int dataCount, i;

  /* Check our record pointer and FIL pointer */
  if (ihexRecord == NULL || in == NULL)
    return IHEX_ERROR_INVALID_ARGUMENTS;

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
  dataCount = strtol(hexBuff, (char **) NULL, 16);

  /* Copy the ASCII hex encoding of the address field into hexBuff, convert it to a usable integer */
  strncpy(hexBuff, recordBuff + IHEX_ADDRESS_OFFSET, IHEX_ADDRESS_LEN);
  hexBuff[IHEX_ADDRESS_LEN] = 0;
  ihexRecord->address = (uint16_t) strtol(hexBuff, (char **) NULL, 16);

  /* Copy the ASCII hex encoding of the address field into hexBuff, convert it to a usable integer */
  strncpy(hexBuff, recordBuff + IHEX_TYPE_OFFSET, IHEX_TYPE_LEN);
  hexBuff[IHEX_TYPE_LEN] = 0;
  ihexRecord->type = strtol(hexBuff, (char **) NULL, 16);

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
    ihexRecord->data[i] = (uint8_t) strtol(hexBuff, (char **) NULL, 16);
  }
  ihexRecord->dataLen = dataCount;

  /* Copy the ASCII hex encoding of the checksum field into hexBuff, convert it to a usable integer */
  strncpy(hexBuff, recordBuff + IHEX_DATA_OFFSET + dataCount * 2,
      IHEX_CHECKSUM_LEN);
  hexBuff[IHEX_CHECKSUM_LEN] = 0;
  ihexRecord->checksum = (uint8_t) strtol(hexBuff, (char **) NULL, 16);

  if (ihexRecord->checksum != Checksum_IHexRecord(ihexRecord))
    return IHEX_ERROR_INVALID_RECORD;

  return IHEX_OK;
}

/* Utility function to calculate the checksum of an Intel HEX8 record */
uint8_t Checksum_IHexRecord(const IHexRecord *ihexRecord) {
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

