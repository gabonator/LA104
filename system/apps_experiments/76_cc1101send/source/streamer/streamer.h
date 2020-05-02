#include "ringbuf.h"
void streamerBegin();
void streamerEnd();
bool isStreaming();
void streamerPause();
void streamerResume();

extern int streamerBufferMaxCounter;
extern const int streamerPeriodUs;
extern volatile int streamerOverrun;

extern RingBufCPP<uint16_t, 256> streamerBuffer;


#ifdef __APPLE__
void streamerBegin(){}
void streamerEnd(){}
bool isStreaming(){return true;}
void streamerPause(){}
void streamerResume(){}
RingBufCPP<uint16_t, 128> streamerBuffer;
#endif
