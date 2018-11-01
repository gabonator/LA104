/*
MD_YX5300 - Library for YX5300 Serial MP3 module

See header file for comments

This file contains class and hardware related methods.

Copyright (C) 2018 Marco Colli. All rights reserved.

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
*/
#include "MD_YX5300.h"

#define PRINTS(...)
#define PRINTX(...)
#define PRINT(...)

/**
 * \file
 * \brief Implements class methods
 */

#define ARRAY_SIZE(a) (sizeof(a)/sizeof(a[0]))  ///< Generic array element count macro

void MD_YX5300::begin(void)
{
  Serial.begin(9600);
  reset();
  delay(500);   // wait for the rest to occur
  device(CMD_OPT_DEV_TF);
  delay(200);   // wait for the file system to be initialized
}

bool MD_YX5300::check(void)
// returns true when received full message or timeout
{
  uint8_t c = '\0';

  // check for timeout if waiting response
  if (_waitResponse && (millis() - _timeSent >= _timeout))
  {
    processResponse(true);
    return(true);
  }

  // check if any characters available
  if (!Serial.available())
    return(false);

  // process all the characters waiting
  do
  {
    c = Serial.read();
 
    if (c == PKT_SOM) _bufIdx = 0;      // start of message - reset the index
    
    _bufRx[_bufIdx++] = c;

    if (_bufIdx >= ARRAY_SIZE(_bufRx))  // keep index within array memory bounds
      _bufIdx = ARRAY_SIZE(_bufRx) - 1;
  } while (Serial.available() && c != PKT_EOM);

  // check if we have a whole message to 
  // process and do something with it here!
  if (c == PKT_EOM)
  {
    processResponse();
  }

  return(c == PKT_EOM);   // we have just processed a response
}

int16_t MD_YX5300::checksum(uint8_t *data, uint8_t len)
{
  int16_t sum = 0;

  for (uint8_t i = 0; i < len; i++)
    sum += data[i];

  return(-sum);
}

bool MD_YX5300::sendRqst(cmdSet_t cmd, uint8_t data1, uint8_t data2)
// returns true if the response status is ready for processing
{
  static uint8_t msg[] = 
  { 
    PKT_SOM,      // 0: Start
    PKT_VER,      // 1: Version
    PKT_LEN,      // 2: Length
    CMD_NUL,      // 3: Command
    PKT_FB_ON,    // 4: Feedback
    PKT_DATA_NUL, // 5: Data Hi
    PKT_DATA_NUL, // 6: Data Lo
#if USE_CHECKSUM
    PKT_DATA_NUL, // [7]: Checksum Hi (optional)
    PKT_DATA_NUL, // [8]: Checksum Lo (optional)
#endif
    PKT_EOM       // 7, [9]: End
  };

  msg[3] = cmd;
  msg[5] = data1;
  msg[6] = data2;

#if USE_CHECKSUM
  int16_t chk = checksum(&msg[1], msg[2]);

  msg[7] = (uint8_t)(chk >> 8);
  msg[8] = (uint8_t)(chk & 0x00ff);
#endif

  Serial.write(msg, ARRAY_SIZE(msg));
  _timeSent = millis();
  _status.code = STS_OK;
  _waitResponse = true;

  // if synchronous mode enabled, wait for a 
  // response message to be processed
  if (_synch)
  {
    do {} while (!check());
    PRINTS("\nSynch wait over");
    return(true);
  }
  
  return(false);
}

void MD_YX5300::processResponse(bool bTimeout)
{
  _waitResponse = false;    // definitely no longer waiting

#if USE_CHECKSUM
  int16_t chk = checksum(&_bufRx[1], _bufRx[2]);
  int16_t chkRcv = ((int16_t)_bufRx[7] << 8) + _bufRx[8];
#endif

  // initialise to most probable message outcome
  _status.code = (status_t)_bufRx[3];
  _status.data = ((uint16_t)_bufRx[5] << 8) | _bufRx[6];

  // now override with message packet errors, if any
  if (bTimeout)
  {
    _status.code = STS_TIMEOUT;
    _status.data = 0;
  }
  else if (_bufRx[1] != PKT_VER)
    _status.code = STS_VERSION;
#if USE_CHECKSUM
  else if (chk != chkRcv)
    _status.code = STS_CHECKSUM;
#endif

  // finally, call the callback if there is one
  if (_cbStatus != nullptr)
    _cbStatus(&_status);
}
