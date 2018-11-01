#pragma once
#include "Arduino.h"

/**
* \file
* \brief Main header file for the MD_YX5300 library
*/

/**
\mainpage Arduino Serial MP3 Player
The MD_YX5300 Library
---------------------
This library implements functions to control RS232 Serial MP3 players based 
on the YX5300 IC. These are often sold as "Catalex" Serial MP3 players and look 
like the device in the photo below. This library manages the serial interface and 
request/response sequences, simplifying running the device as a background task.

![YX5300 Serial MP3 Player] (YX5300_Serial_MP3_Player.jpg "YX5300 Serial MP3 Player")

The YX5300 supports 8kHz to 48kHz sampling frequency MP3 and WAV file formats. 
The audio files are stored on a micro SD card that plugs into a TF card socket on 
the back of the board. The MCU controls the MP3 playback by sending serial commands 
to the module via the UART port.

Module Interfaces
-----------------
![YX5300 Catalex Layout] (YX5300_Catalex_Layout.png "YX5300 Catalex Module Layout")

- __Control interface__ is a UART TTL interface (GND, VCC, TX, RX). Pins are connected 
GND to MCU ground, VCC to 5V power supply, YX5300 TX (transmit) to the designated 
RX (receive) pin for the SoftwareSerial library, YX5300 RX to SoftwareSerial TX.
- __TF card socket__ on the reverse side of the PCB for plugging in the micro SD 
card with MP3/WAV files.
- __Playback indicator__ (green LED) blinks during playback, steady otherwise.
- __Headphone jack__ for sound output to headphones or external amplifier.

MP3/WAV File
------------
The micro SD card should be formatted as FAT16 or FAT32.

Songs must be prefixed with a unique 3 digit index number. For example
001xxx.mp3, 002xxx.mp3, 003xxx.mp3 (where xxx is an arbitrary optional
name). Songs may also be arranged in folders named '01', '02', '03', 
etc. Even if you plan only one play list, it is better to keep them in 
a '01' folder.

An example of the folder and files on the micro SD card might look like:

~~~~
+-+- 01
  |   + 001-Happy_Dance.mp3
  |   + 002-O_Sole_Mio.mp3
  |
  +- 02
  |   + 003-Humpty_Dumpty.mp3
  |   + 004-Incy_Wincy_Spider.mp3
  |   + 005-Grand_Duke.mp3
  | 
  +- 03
      + 006-Fernando.mp3
      + 007-Mamma_Mia.mp3
~~~~

Library Dependencies
--------------------
The library used the _SoftwareSerial_ library to manage the serial interface to the
MP3 player. If more than one hardware Serial port is available this may be used but 
requires changing the alternative code switched by the C++ macro define 
USE_SOFTWARESERIAL at the top of the library header file.

Topics
------
- \subpage pageSoftware
- \subpage pageRevisionHistory
- \subpage pageCopyright
- \subpage pageDonation
- \subpage pageOtherLinks

Known Issues
------------
- equalizer() command is accepted but not actioned by device. Documentation seems to indicate that
this function may be disabled in the hardware (Chinese translation is ambiguous).
- playFolderShuffle() command is accepted but not actioned by the device.
- shuffle() command is accepted but not seem to shuffle files playback.

\page pageDonation Support the Library
If you like and use this library please consider making a small donation using [PayPal](https://paypal.me/MajicDesigns/4USD)

\page pageOtherLinks Other Useful Links
The Catalex documentation for their modules:
- Catalex_YX5300_Docs.zip files (Chinese) http://pan.baidu.com/s/1hqilpB2

This library also integrates data and ideas from a number of other places:
- (French) https://andrologiciels.wordpress.com/arduino/son-et-arduino/mp3/catalex-mp3-serie/
- (Polish) http://www.jarzebski.pl/arduino/komponenty/modul-mp3-z-ukladem-yx5300.html
- (French) https://www.carnetdumaker.net/articles/utiliser-un-lecteur-serie-de-fichiers-mp3-avec-une-carte-arduino-genuino/

\page pageSoftware Message Flow Management

![YX5300 Board Layout] (YX5300_Board_Layout.jpg "YX5300 Board Layout")

Communications Format
---------------------
The MP3 module communicates using asynchronous RS232 serial communication at
9600 bps, 8 data bits, No parity, 1 stop bit, no flow control.

Flow control is implemented using a serial RQST/RESP protocol based on data packets
with the following byte format:

|Packet Format (bytes) |||||||||
|-------|---------|--------|---------|----------|--------|--------|---------|---------|-----|
| Start | Version | Length | Command | Feedback | DataHi | DataLo | [ChkHi] | [ChkLo] | End |


|Byte      | Value | Description
|:---------|------:|:----------------------------------------------------|
| Start    | 0x7e  | The start of each packet, used for synchronization. |
| Version  | 0xff  | Always the same value in this implementation.       |
| Length   | 0x06  | Number of bytes between Start and Chk.              |
| Command  | 0x??  | Command code for required action.                   |
| Feedback | 0x01  | Set to 1 for protocol feedback, 0 for none.         |
| DataHi   | 0x??  | Length-4 data bytes = 2 in this implementation.     |
| DataLo   | 0x??  | ^                                                   |
| ChkHi    | 0x??  | Optional checksum for bytes between Start and Chk.  |
| ChkLo    | 0x??  | ^                                                   |
| End      | 0xef  | The end of each packet.                             |

The message flow between the device and MCU is be displayed on the Serial 
Monitor by the library when the C++ macro define LIBDEBUG is set to 1 in 
the main code file.

The checksum is optional in the protocol packet. The library will use the 
checksum field if the C++ macro define USE_CHECKSUM is set to 1 in the header
file.

Sending and Receiving Serial Messages
-------------------------------------
This library manages the serial interface to the TX5300 by taking care of
sending and receiving the serial messages. The MP3 Player responds to 
command requests but it also sends unsolicited messages when certain events 
occur (eg, TF card removed or inserted). These unsolicited messages may be 
important to the application so the library implements mechanisms that 
allow the application to process the relevant notification data by placing 
it in a cbData structure. 

How the cbData structure is received by the application is flexible and 
depends on the setSynchronous() setting and whether a callback is defined. 
This is summarized in the table below and explained in the text that follows.

|       | Callback           | Polled             |
|------:|:-------------------|:-------------------|
| Sync  | _unsol_: callback  | _unsol_: check()   |
| ^     | _resp_: ret status | _resp_: ret status |
| Async | _unsol_: callback  | _unsol_: check()   |
| ^     | _resp_: callback   | _resp_: check()    |

The first choice is whether to process the message in line with the calling
sequence (synchronous) or separately (asynchronously):
- __Synchronous__: The command message is sent and the code waits for the 
response before returning to the calling application. This is relatively
inefficient of CPU time as it involves a busy wait, but is easy to implement
in the application and is good for simple applications
- __Asynchronous__: The command message is sent and the library immediately 
returns. The response message is processed as it returns and the calling
application can continue to run while this happens. Once the response is 
received, the calling can be notified through a callback or polled status
(see below).

Independently of the sync/async mode, the application can choose to be 
informed received messages are ready to process either by:
- __Polling__: The return status of the check() method is used as the signal 
that an unsolicited message has been received. In synchronous mode the return 
code for the method invoked will be the status of check() for that request.
- __Callback__: If a callback function is defined  (see setCallback()), every 
unsolicited message received will be processed through the callback mechanism.
In Synchronous mode, both the callback and the return code for the method 
invoked will signal receipt of the same message, so the application code should
guard against processing the message twice.

\page pageCopyright Copyright
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

\page pageRevisionHistory Revision History
Jul 2018 version 1.0.0
- First Release
*/

/**
* Core object for the MD_YX5300 library
*/
class MD_YX5300
{
public:
  /**
  * Status code enumerated type specification.
  *
  * Used by the cbData status structure in the code field to 
  * identify the type of status data contained.
  */
  enum status_t
  {
    STS_OK = 0x00,         ///< No error (library generated status)
    STS_TIMEOUT = 0x01,    ///< Timeout on response message (library generated status)
    STS_VERSION = 0x02,    ///< Wrong version number in return message (library generated status)
    STS_CHECKSUM = 0x03,   ///< Device checksum invalid (library generated status)
    STS_TF_INSERT = 0x3a,  ///< TF Card was inserted (unsolicited)
    STS_TF_REMOVE = 0x3b,  ///< TF card was removed (unsolicited)
    STS_FILE_END = 0x3d,   ///< Track/file has ended (unsolicited)
    STS_INIT = 0x3f,       ///< Initialization complete (unsolicited)
    STS_ERR_FILE = 0x40,   ///< Error file not found
    STS_ACK_OK = 0x41,     ///< Message acknowledged ok
    STS_STATUS = 0x42,     ///< Current status
    STS_VOLUME = 0x43,     ///< Current volume level
    STS_EQUALIZER = 0x44,  ///< Equalizer status
    STS_TOT_FILES = 0x48,  ///< TF Total file count
    STS_PLAYING = 0x4c,    ///< Current file playing
    STS_FLDR_FILES = 0x4e, ///< Total number of files in the folder
    STS_TOT_FLDR = 0x4f,   ///< Total number of folders
  };

 /**
  * Status return structure specification.
  *
  * Used to return (through callback or getStatus() method) the
  * status value of the last device request.
  *
  * Device commands will always receive a STS_ACK_OK if the message was received
  * correctly. Some commands, notably query requests, will also include a response
  * with status or information data. These methods are listed below:
  *
  * | Method             | Return Status (code) | Return Data (data)                        
  * |:-------------------|:---------------------|:--------------------
  * | Unsolicited mesg   | STS_FILE_END         | Index number of the file just completed.
  * | Unsolicited mesg   | STS_INIT             | Device initialization complete - file store types available (0x02 for TF).
  * | Unsolicited mesg   | STS_ERR_FILE         | File index
  * | queryStatus()      | STS_STATUS           | Current status. High byte is file store (0x02 for TF); low byte 0x00=stopped, 0x01=play, 0x02=paused.
  * | queryVolume()      | STS_VOLUME           | Current volume level [0..MAX_VOLUME].
  * | queryFilesCount()  | STS_TOT_FILES        | Total number of files on the TF card.
  * | queryFile()        | STS_PLAYING          | Index number of the current file playing.
  * | queryFolderFiles() | STS_FLDR_FILES       | Total number of files in the folder.
  * | queryFolderCount() | STS_TOT_FLDR         | Total number of folders on the TF card.
  * | queryEqualizer()   | STS_EQUALIZER        | Current equalizer mode [0..5].
  */
  struct cbData
  {
    status_t code;    ///< code for operation
    uint16_t data;    ///< data returned
  };

  /**
  * Class Constructor.
  *
  * Instantiate a new instance of the class. The parameters passed are used to
  * connect the software to the hardware.
  *
  * Parameters are required for SoftwareSerial initialization. If native
  * serial port is used then dummy parameters need to be supplied.
  *
  * \param pinRx The pin for receiving serial data, connected to the device Tx pin.
  * \param pinTx The pin for sending serial data, connected to the device Rx pin.
  */
  MD_YX5300() :
  _timeout(200), _synch(true), _cbStatus(nullptr)
    {};

 /**
  * Class Destructor.
  *
  * Released any necessary resources and and does the necessary to clean up once 
  * the object is no longer required.
  */
  //~MD_YX5300(void) {};
  
 /**
  * Initialize the object.
  *
  * Initialize the object data. This needs to be called during setup() to initialize
  * new data for the class that cannot be done during the object creation.
  *
  * The MP3 device is reset and the TF card set as the input file system, with 
  * appropriate delays after each operation.
  */
  void begin(void);

 /**
  * Receive and process serial messages.
  *
  * The check function should be called repeatedly in loop() to allow the 
  * library to receive and process device messages. The MP3 device can send
  * messages as a response to a request or unsolicited to inform of state changes,
  * such a track play completing. A true value returned indicates that a message 
  * has been received and the status is ready to be processed. 
  * 
  * With callbacks disabled, the application should use getStatus() to retrieve 
  * and process this status. With callbacks enabled, the check() will cause the 
  * callback to be processed before returning to the application.
  *
  * If synchronous mode is enabled only unsolicited messages will be processed
  * through the call to check(), as the other messages will have been processed 
  * synchronously as part of the request.
  *
  * \sa cbData, setCallback(), setSynchronous(), getStatus()
  *
  * \return true if a message has been received and processed, false otherwise.
  */
  bool check(void);

  //--------------------------------------------------------------
  /** \name Methods for object management.
  * @{
  */
  
 /**
  * Set or clear Synchronous mode.
  *
  * Set/reset synchronous mode operation for the library. In synchronous mode,
  * the library will wait for device response message immediately after sending 
  * the request. On return the status result of the operation will be available
  * using the getStatus() method. If synchronous mode is disabled, then the code 
  * must be retrieved using getStatus() when the check() method returns true.
  *
  * Synchronous mode and callbacks are set and operate independently.
  *
  * \sa getStatus(), check(), setCallback()
  *
  * \param b true to set the mode, false to disable.
  * \return No return value.
  */
  inline void setSynchronous(bool b) { _synch = b; }

 /**
  * Set serial response timeout.
  *
  * Set the device response timeout in milliseconds. If a message is not received
  * within this time a timeout error status will be generated. The default timeout
  * is 200 milliseconds.
  *
  * \param t timeout in milliseconds
  * \return No return value.
  */
  inline void setTimeout(uint32_t t) { _timeout = t; }

 /**
  * Set the status callback.
  *
  * Set status callback function that will be invoked when a device status serial
  * message is received. The callback will include the status encoded in the serial 
  * message. If callback is not required, the callback should be set to nullptr 
  * (the default). The callback is invoked when the last character of response message
  * is received from the device.
  *
  * Callbacks and synchronous mode are set and operate independently.
  *
  * \sa getStatus(), setSynchronous()
  *
  * \param cb the address of the callback function in user code.
  * \return No return value.
  */
  inline void setCallback(void(*cb)(const cbData *data)) { _cbStatus = cb; }

 /**
  * Get the status of last device operation.
  *
  * Get the status of the last requested operation of the MP3 device. A pointer
  * to the library's status block is returned to the application. The status code is 
  * one of the status_t enumerated types and the data component returned will depend
  * status value. The code and data depend on the original request made of the device.
  * 
  * \sa cbData, getStsCode(), getStsData()
  *
  * \return Pointer to the 'last status' data structure.
  */
  inline const cbData *getStatus(void) { return(&_status); }

 /**
  * Get the status code of last device operation.
  *
  * Get the status code of the of the last MP3 operation. The status code is
  * one of the status_t enumerated types. 
  *
  * \sa cbData, getStatus(), getStsData()
  *
  * \return The requested status code.
  */
  inline status_t getStsCode(void) { return(_status.code); }

 /**
  * Get the status data of last device operation.
  *
  * Get the status data of the of the last MP3 operation. The meaning of the
  * data will depend on the associated status code.
  *
  * \sa cbData, getStatus(), getStsCode()
  *
  * \return The requested status data.
  */
  inline uint16_t getStsData(void) { return(_status.data); }

  /** @} */

  //--------------------------------------------------------------
  /** \name Methods for device management.
  * @{
  */

 /**
  * Set the file store device.
  *
  * Set the file store device to the specified type. Currently the only type 
  * available is a TF device (CMD_OPT_TF). The application should allow 200ms 
  * for the file system to be initialized before further interacting with the 
  * MP3 device.
  *
  * The TF file system is set at begin() and this method should not need to 
  * be called from application code.
  *
  * \sa check(), getStatus(), setSynchronous()
  *
  * \param devId the device id for the type of device. Currently only 0x02 (TF).
  * \return In synchronous mode, true when the message has been received and processed. Otherwise
  *         ignore the return value and process using callback or check() and getStatus().
  */
  inline bool device(uint8_t devId) { return sendRqst(CMD_SEL_DEV, PKT_DATA_NUL, devId); }


 /**
  * Set equalizer mode.
  *
  * Set the equalizer mode to one of the preset types - 
  * 0:Normal, 1:Pop, 2:Rock, 3:Jazz, 4:Classic or 5:Base
  * 
  * \sa check(), getStatus(), setSynchronous()
  *
  * \param eqId the equalizer type requested [1..5].
  * \return In synchronous mode, true when the message has been received and processed. Otherwise
  *         ignore the return value and process using callback or check() and getStatus().
  */
  inline bool equalizer(uint8_t eqId) { return sendRqst(CMD_SET_EQUALIZER, PKT_DATA_NUL, eqId <= 5 ? eqId : 0); }

  /**
  * Set sleep mode.
  *
  * Enables the MP3 player sleep mode. The device will stop playing but still respond to 
  * serial messages. Use wakeUp() to disable sleep mode.
  *
  * \sa wakeUp(), check(), getStatus(), setSynchronous()
  *
  * \return In synchronous mode, true when the message has been received and processed. Otherwise
  *         ignore the return value and process using callback or check() and getStatus().
  */
  inline bool sleep(void) { return sendRqst(CMD_SLEEP_MODE, PKT_DATA_NUL, PKT_DATA_NUL); }

 /**
  * Set awake mode.
  *
  * Wakes up the MP3 player from sleep mode. Use sleep() to enable sleep mode.
  *
  * \sa sleep(), check(), getStatus(), setSynchronous()
  *
  * \return In synchronous mode, true when the message has been received and processed. Otherwise
  *         ignore the return value and process using callback or check() and getStatus().
  */
  inline bool wakeUp(void) { return sendRqst(CMD_WAKE_UP, PKT_DATA_NUL, PKT_DATA_NUL); }

  /**
  * Control shuffle playing mode.
  *
  * Set or reset the playing mode to/from random shuffle.
  * At the end of playing each file the device will send a STS_FILE_END message.
  *
  * \sa check(), getStatus(), setSynchronous()
  *
  * \param b true to enable mode, false to disable.
  * \return In synchronous mode, true when the message has been received and processed. Otherwise
  *         ignore the return value and process using callback or check() and getStatus().
  */
  inline bool shuffle(bool b) { return sendRqst(CMD_SHUFFLE_PLAY, PKT_DATA_NUL, b ? CMD_OPT_ON : CMD_OPT_OFF); }

  /**
  * Control repeat play mode (current file).
  *
  * Set or reset the repeat playing mode for the currently playing track.
  * At the end of each repeat play the device will send a STS_FILE_END message.
  *
  * \sa check(), getStatus(), setSynchronous()
  *
  * \param b true to enable mode, false to disable.
  * \return In synchronous mode, true when the message has been received and processed. Otherwise
  *         ignore the return value and process using callback or check() and getStatus().
  */
  inline bool repeat(bool b) { return sendRqst(CMD_SET_SNGL_CYCL, PKT_DATA_NUL, b ? CMD_OPT_ON : CMD_OPT_OFF); }

  /**
  * Reset the MP3 player.
  *
  * Put the MP3 player into reset mode. The payer will return to its power up state.
  * The application should allow 500ms between the rest command and any subsequent 
  * interaction with the device.
  *
  * \sa check(), getStatus(), setSynchronous()
  *
  * \return In synchronous mode, true when the message has been received and processed. Otherwise
  *         ignore the return value and process using callback or check() and getStatus().
  */
  inline bool reset(void) { return sendRqst(CMD_RESET, PKT_DATA_NUL, PKT_DATA_NUL); }

  /** @} */

  //--------------------------------------------------------------
 /** \name Methods for controlling playing MP3 files.
  * @{
  */

 /**
  * Play the next MP3 file.
  * 
  * Play the next MP3 file in numeric order.
  * At the end of playing the file the device will send a STS_FILE_END message.
  *
  * \sa check(), getStatus(), setSynchronous()
  *
  * \return In synchronous mode, true when the message has been received and processed. Otherwise
  *         ignore the return value and process using callback or check() and getStatus().
  */
  inline bool playNext(void) { return sendRqst(CMD_NEXT_SONG, PKT_DATA_NUL, PKT_DATA_NUL); }

 /**
  * Play the previous MP3 file.
  *
  * Play the previous MP3 file in numeric order.
  * At the end of playing the file the device will send a STS_FILE_END message.
  *
  * \sa check(), getStatus(), setSynchronous()
  *
  * \return In synchronous mode, true when the message has been received and processed. Otherwise
  *         ignore the return value and process using callback or check() and getStatus().
  */
  inline bool playPrev(void) { return sendRqst(CMD_PREV_SONG, PKT_DATA_NUL, PKT_DATA_NUL); }

 /**
  * Stop playing the current MP3 file.
  *
  * Stop playing the current MP3 file and cancel the current playing mode.
  * playPause() should be used for a temporary playing stop.
  *
  * \sa playPause(), check(), getStatus(), setSynchronous()
  *
  * \return In synchronous mode, true when the message has been received and processed. Otherwise
  *         ignore the return value and process using callback or check() and getStatus().
  */
  inline bool playStop(void) { return sendRqst(CMD_STOP_PLAY, PKT_DATA_NUL, PKT_DATA_NUL); }

 /**
  * Pause playing the current MP3 file.
  *
  * Pause playing playing the current MP3 file. playStart() should follow to restart
  * the same MP3 file. playStop() should be use to stop playing and abort current 
  * playing mode.
  *
  * \sa playPause(), playStart(), check(), getStatus(), setSynchronous()
  *
  * \return In synchronous mode, true when the message has been received and processed. Otherwise
  *         ignore the return value and process using callback or check() and getStatus().
  */
  inline bool playPause(void) { return sendRqst(CMD_PAUSE, PKT_DATA_NUL, PKT_DATA_NUL); }

 /**
  * Restart playing the current MP3 file.
  *
  * Restart playing playing the current MP3 file after a playPause().
  * At the end of playing the file the device will send a STS_FILE_END message.
  *
  * \sa playPause(), check(), getStatus(), setSynchronous()
  *
  * \return In synchronous mode, true when the message has been received and processed. Otherwise
  *         ignore the return value and process using callback or check() and getStatus().
  */
  inline bool playStart(void) { return sendRqst(CMD_PLAY, PKT_DATA_NUL, PKT_DATA_NUL); }

 /**
  * Play a specific file.
  *
  * Play a file by specifying the file index number.
  * At the end of playing the file the device will send a STS_FILE_END message.
  *
  * \sa check(), getStatus(), setSynchronous()
  *
  * \param t the file indexed (0-255) to be played.
  * \return In synchronous mode, true when the message has been received and processed. Otherwise
  *         ignore the return value and process using callback or check() and getStatus().
  */
  inline bool playTrack(uint8_t t) { return sendRqst(CMD_PLAY_WITH_INDEX, PKT_DATA_NUL, t); }

  /**
  * Play repeat specific track.
  *
  * Play a track in repeat mode.
  * At the end of playing the file the device will send a STS_FILE_END message.
  *
  * \sa check(), getStatus(), setSynchronous()
  *
  * \param file the file index.
  * \return In synchronous mode, true when the message has been received and processed. Otherwise
  *         ignore the return value and process using callback or check() and getStatus().
  */
  inline bool playTrackRepeat(uint8_t file) { return sendRqst(CMD_SNG_CYCL_PLAY, PKT_DATA_NUL, file); }

  /**
  * Play a specific file in a folder.
  *
  * Play a file by specifying the folder and file to be played.
  * At the end of playing the file the device will send a STS_FILE_END message.
  *
  * \sa check(), getStatus(), setSynchronous()
  *
  * \param fldr the folder number containing the files.
  * \param file the file within the folder.
  * \return In synchronous mode, true when the message has been received and processed. Otherwise
  *         ignore the return value and process using callback or check() and getStatus().
  */
  inline bool playSpecific(uint8_t fldr, uint8_t file) { return sendRqst(CMD_PLAY_FOLDER_FILE, fldr, file); }

 /**
  * Control repeat play mode (specific folder).
  *
  * Set or reset the repeat playing mode for the specified folder.
  * At the end of playing each file the device will send a STS_FILE_END message.
  *
  * \sa check(), getStatus(), setSynchronous()
  *
  * \param folder the folder number containing the files to repeat.
  * \return In synchronous mode, true when the message has been received and processed. Otherwise
  *         ignore the return value and process using callback or check() and getStatus().
  */
  inline bool playFolderRepeat(uint8_t folder) { return sendRqst(CMD_FOLDER_CYCLE, folder, PKT_DATA_NUL); }

  /**
  * Control shuffle play mode (specific folder).
  *
  * Set or reset the shuffle playing mode for the specified folder.
  * At the end of playing each file the device will send a STS_FILE_END message.
  *
  * \sa check(), getStatus(), setSynchronous()
  *
  * \param folder the folder number containing the files to repeat.
  * \return In synchronous mode, true when the message has been received and processed. Otherwise
  *         ignore the return value and process using callback or check() and getStatus().
  */
  inline bool playFolderShuffle(uint8_t folder) { return sendRqst(CMD_SHUFFLE_FOLDER, folder, PKT_DATA_NUL); }

  /** @} */

  //--------------------------------------------------------------
  /** \name Methods for controlling MP3 output volume.
  * @{
  */

 /**
  * Set specified volume.
  *
  * Set the output volume to the specified level. Maximum volume is 30.
  *
  * \sa volumeInc(), volumeDec(), check(), getStatus(), setSynchronous()
  *
  * \param vol the volume specified [0..30].
  * \return In synchronous mode, true when the message has been received and processed. Otherwise
  *         ignore the return value and process using callback or check() and getStatus().
  */
  inline bool volume(uint8_t vol) { return sendRqst(CMD_SET_VOLUME, PKT_DATA_NUL, (vol > MAX_VOLUME ? MAX_VOLUME : vol)); }

 /**
  * Increment the volume.
  *
  * Increment the output volume by 1.
  *
  * \sa volume(), volumeDec(), check(), getStatus(), setSynchronous()
  *
  * \return In synchronous mode, true when the message has been received and processed. Otherwise
  *         ignore the return value and process using callback or check() and getStatus().
  */
  inline bool volumeInc(void) { return sendRqst(CMD_VOLUME_UP, PKT_DATA_NUL, PKT_DATA_NUL); }

 /**
  * Decrement the volume.
  *
  * Decrement the output volume by 1.
  *
  * \sa volume(), volumeInc(), check(), getStatus(), setSynchronous()
  *
  * \return In synchronous mode, true when the message has been received and processed. Otherwise
  *         ignore the return value and process using callback or check() and getStatus().
  */
  inline bool volumeDec(void) { return sendRqst(CMD_VOLUME_DOWN, PKT_DATA_NUL, PKT_DATA_NUL); }

 /**
  * Mute the sound output.
  *
  * Mute the sound output by suppressing the output from the DAC. The MP3 file
  * will continue playing but will not be heard. To temporarily halt the playing use
  * playPause().
  *
  * \sa playPause(), check(), getStatus(), setSynchronous()
  *
  * \param b true to enable mode, false to disable.
  * \return In synchronous mode, true when the message has been received and processed. Otherwise
  *         ignore the return value and process using callback or check() and getStatus().
  */
  inline bool volumeMute(bool b) { return sendRqst(CMD_SET_DAC, PKT_DATA_NUL, b ? CMD_OPT_OFF : CMD_OPT_ON); }

 /**
  * Query the current volume setting.
  *
  * Request the current volume setting from the device. This is a wrapper alternative 
  * for queryVolume().
  *
  * \sa queryVolume(), check(), getStatus(), setSynchronous()
  *
  * \return In synchronous mode, true when the message has been received and processed. Otherwise
  *         ignore the return value and process using callback or check() and getStatus().
  */
  inline bool volumeQuery(void) { return sendRqst(CMD_QUERY_VOLUME, PKT_DATA_NUL, PKT_DATA_NUL); }

  /** @} */

  //--------------------------------------------------------------
  /** \name Methods for querying MP3 device parameters.
  * @{
  */
  /**
  * Query the current status.
  *
  * Request the current status setting from the device.
  *
  * - cbData.code is STS_STATUS.
  * - cbData.data high byte is the active file store active (0x02 for TF); 
  * low byte 0x00=stopped, 0x01=play, 0x02=paused.
  *
  * \sa volumeQuery(), check(), getStatus(), setSynchronous()
  *
  * \return In synchronous mode, true when the message has been received and processed. Otherwise
  *         ignore the return value and process using callback or check() and getStatus().
  */
  inline bool queryStatus(void) { return sendRqst(CMD_QUERY_STATUS, PKT_DATA_NUL, PKT_DATA_NUL); }

 /**
  * Query the current volume setting.
  *
  * Request the current volume setting from the device. This is a wrapper alternative
  * for volumeQuery().
  *
  * - cbData.code is STS_VOLUME.
  * - cbData.data is the volume setting.
  *
  * \sa volumeQuery(), check(), getStatus(), setSynchronous()
  *
  * \return In synchronous mode, true when the message has been received and processed. Otherwise
  *         ignore the return value and process using callback or check() and getStatus().
  */
  inline bool queryVolume(void) { return volumeQuery(); }

  /**
  * Query the current equalizer setting.
  *
  * Request the current equalizer setting from the device.
  *
  * - cbData.code is STS_??.
  * - cbData.data is the equalizer setting.
  *
  * \sa check(), getStatus(), setSynchronous()
  *
  * \return In synchronous mode, true when the message has been received and processed. Otherwise
  *         ignore the return value and process using callback or check() and getStatus().
  */
  inline bool queryEqualizer(void) { return sendRqst(CMD_QUERY_EQUALIZER, PKT_DATA_NUL, PKT_DATA_NUL); }

 /**
  * Query the number of files in the specified folder.
  *
  * Request the count of files in the specified folder number.
  *
  * - cbData.code is STS_FLDR_FILES.
  * - cbData.data is the count of the files in the folder.
  *
  * \sa check(), getStatus(), setSynchronous()
  *
  * \param folder the folder number whose files are to be counted.
  * \return In synchronous mode, true when the message has been received and processed. Otherwise
  *         ignore the return value and process using callback or check() and getStatus().
  */
  inline bool queryFolderFiles(uint8_t folder) { return sendRqst(CMD_QUERY_FLDR_FILES, PKT_DATA_NUL, folder); }

 /**
  * Query the total number of folders.
  *
  * Request the count of folder on the TF device.
  *
  * - cbData.code is STS_TOT_FLDR.
  * - cbData.data is the count of the folder on the file store, including
  * the root folder.
  *
  * \sa check(), getStatus(), setSynchronous()
  *
  * \return In synchronous mode, true when the message has been received and processed. Otherwise
  *         ignore the return value and process using callback or check() and getStatus().
  */
  inline bool queryFolderCount(void) { return sendRqst(CMD_QUERY_TOT_FLDR, PKT_DATA_NUL, PKT_DATA_NUL); }

 /**
  * Query the total number of files.
  *
  * Request the count of files on the TF device.
  *
  * - cbData.code is STS_TOT_FILES.
  * - cbData.data is the count of the files on the file store.
  *
  * \sa check(), getStatus(), setSynchronous()
  *
  * \return In synchronous mode, true when the message has been received and processed. Otherwise
  *         ignore the return value and process using callback or check() and getStatus().
  */
  inline bool queryFilesCount(void) { return sendRqst(CMD_QUERY_TOT_FILES, PKT_DATA_NUL, PKT_DATA_NUL); }

 /**
  * Query the file currently playing.
  *
  * Request the index of the file currently being played.
  *
  * - cbData.code is STS_PLAYING.
  * - cbData.data is the index of the file currently playing.
  *
  * \sa check(), getStatus(), setSynchronous()
  *
  * \return In synchronous mode, true when the message has been received and processed. Otherwise
  *         ignore the return value and process using callback or check() and getStatus().
  */
  inline bool queryFile(void) { return sendRqst(CMD_QUERY_PLAYING, PKT_DATA_NUL, PKT_DATA_NUL); }

  /** @} */

private:
  // Miscellaneous
  const uint8_t MAX_VOLUME = 30;  // Maximum allowed volume setting

  // Enumerated type for serial message commands
  enum cmdSet_t
  {
    CMD_NUL = 0x00,             ///< No command
    CMD_NEXT_SONG = 0x01,       ///< Play next song
    CMD_PREV_SONG = 0x02,       ///< Play previous song
    CMD_PLAY_WITH_INDEX = 0x03, ///< Play song with index number
    CMD_VOLUME_UP = 0x04,       ///< Volume increase by one
    CMD_VOLUME_DOWN = 0x05,     ///< Volume decrease by one
    CMD_SET_VOLUME = 0x06,      ///< Set the volume to level specified
    CMD_SET_EQUALIZER = 0x07,   ///< Set the equalizer to specified level
    CMD_SNG_CYCL_PLAY = 0x08,   ///< Loop play (repeat) specified track
    CMD_SEL_DEV = 0x09,         ///< Select storage device to TF card
    CMD_SLEEP_MODE = 0x0a,      ///< Chip enters sleep mode
    CMD_WAKE_UP = 0x0b,         ///< Chip wakes up from sleep mode
    CMD_RESET = 0x0c,           ///< Chip reset
    CMD_PLAY = 0x0d,            ///< Playback restart
    CMD_PAUSE = 0x0e,           ///< Playback is paused
    CMD_PLAY_FOLDER_FILE = 0x0f,///< Play the song with the specified folder and index number
    CMD_STOP_PLAY = 0x16,       ///< Playback is stopped
    CMD_FOLDER_CYCLE = 0x17,    ///< Loop playback from specified folder
    CMD_SHUFFLE_PLAY = 0x18,    ///< Playback shuffle mode
    CMD_SET_SNGL_CYCL = 0x19,   ///< Set loop play (repeat) on/off for current file
    CMD_SET_DAC = 0x1a,         ///< DAC on/off control
    CMD_PLAY_W_VOL = 0x22,      ///< Play track at the specified volume
    CMD_SHUFFLE_FOLDER = 0x28,  ///< Playback shuffle mode for folder specified
    CMD_QUERY_STATUS = 0x42,    ///< Query Device Status
    CMD_QUERY_VOLUME = 0x43,    ///< Query Volume level
    CMD_QUERY_EQUALIZER = 0x44, ///< Query current equalizer (disabled in hardware)
    CMD_QUERY_TOT_FILES = 0x48, ///< Query total files in all folders
    CMD_QUERY_PLAYING = 0x4c,   ///< Query which track playing
    CMD_QUERY_FLDR_FILES = 0x4e,///< Query total files in folder
    CMD_QUERY_TOT_FLDR = 0x4f,  ///< Query number of folders
  };
  
  // Command options
  const uint8_t CMD_OPT_ON = 0x00;    ///< On indicator
  const uint8_t CMD_OPT_OFF = 0x01;   ///< Off indicator
  const uint8_t CMD_OPT_DEV_UDISK = 0X01; ///< Device option UDisk (not used)
  const uint8_t CMD_OPT_DEV_TF = 0X02;    ///< Device option TF
  const uint8_t CMD_OPT_DEV_FLASH = 0X04; ///< Device option Flash (not used)

  // Protocol Message Characters
  const uint8_t PKT_SOM = 0x7e;       ///< Start of message delimiter character
  const uint8_t PKT_VER = 0xff;       ///< Version information
  const uint8_t PKT_LEN = 0x06;       ///< Data packet length in bytes (excluding SOM, EOM)
  const uint8_t PKT_FB_OFF = 0x00;    ///< Command feedback OFF
  const uint8_t PKT_FB_ON = 0x01;     ///< Command feedback ON
  const uint8_t PKT_DATA_NUL = 0x00;  ///< Packet data place marker 
  const uint8_t PKT_EOM = 0xef;       ///< End of message delimiter character

  // variables
  uint32_t _timeout{200};  ///< timeout for return serial message
  bool _synch{true};        ///< synchronous (wait for response) if true

  void(*_cbStatus)(const cbData *data){nullptr}; ///< callback function
  cbData _status;     ///< callback status data

  uint8_t _bufRx[30]; ///< receive buffer for serial comms
  uint8_t _bufIdx;    ///< index for next char into _bufIdx
  uint32_t _timeSent; ///< time last serial message was sent
  bool _waitResponse; ///< true when we are waiting response to a query

  // Methods
  int16_t checksum(uint8_t *data, uint8_t len);               ///< Protocol packet checksum calculation
  bool sendRqst(cmdSet_t cmd, uint8_t data1, uint8_t data2);  ///< Send serial message (Rqst)
  void processResponse(bool bTimeout = false);                ///< Process a serial response message
};
