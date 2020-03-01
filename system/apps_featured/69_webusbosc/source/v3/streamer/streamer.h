#include "ringbuf.h"

//void adcConfigure();
//void adcReset();
//void streamerInit();
//void streamerDeinit();
//void streamerStart();
//void streamerStop();
void streamerBegin();
void streamerEnd();
bool isStreaming();
void streamerPause();
void streamerResume();



extern RingBufCPP<uint8_t, 2048+256> streamerBuffer;