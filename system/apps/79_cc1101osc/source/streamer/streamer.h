#include "ringbuf.h"
void streamerBegin();
void streamerEnd();
bool isStreaming();
void streamerPause();
void streamerResume();

#ifndef __APPLE__

extern int streamerBufferMaxCounter;
extern volatile int streamerOverrun;
extern RingBuf<uint16_t, 512> streamerBuffer;
extern uint16_t streamerSecondary[64];
extern uint32_t streamerSecondaryIndex;
extern volatile int streamerFlag;
extern uint32_t streamerTick;
extern uint32_t streamerLastTimestamp;
extern uint32_t streamerLastInterval;

#else
void streamerBegin(){}
void streamerEnd(){}
bool isStreaming(){return true;}
void streamerPause(){}
void streamerResume(){}
RingBuf<uint16_t, 512> streamerBuffer;
volatile int streamerOverrun;
int streamerBufferMaxCounter;
uint16_t streamerSecondary[64];
uint32_t streamerSecondaryIndex;
uint32_t streamerTick;

#endif
