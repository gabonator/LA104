#include "ringbuf.h"
void streamerBegin();
void streamerEnd();
bool isStreaming();
void streamerPause();
void streamerResume();

#ifndef __APPLE__

extern int streamerBufferMaxCounter;
extern const int streamerPeriodUs;
extern volatile int streamerOverrun;
extern RingBufCPP<uint16_t, 256> streamerBuffer;

#else
void streamerBegin(){}
void streamerEnd(){}
bool isStreaming(){return true;}
void streamerPause(){}
void streamerResume(){}
RingBufCPP<uint16_t, 256> streamerBuffer;
volatile int streamerOverrun;
int streamerBufferMaxCounter;

#endif
