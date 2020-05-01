#include "ringbuf.h"
void streamerBegin();
void streamerEnd();
bool isStreaming();
void streamerPause();
void streamerResume();

extern RingBufCPP<uint16_t, 128> streamerBuffer;