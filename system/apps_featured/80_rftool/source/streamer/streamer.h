#include <stdint.h>

void streamerBegin();
void streamerEnd();
bool isStreaming();
void streamerPause();
void streamerResume();

#ifndef __APPLE__

extern int streamerBufferMaxCounter;
extern const int streamerPeriodUs;
extern volatile int streamerOverrun;
extern RingBufCPP<uint16_t, 512> streamerBuffer;
extern uint16_t streamerSecondary[64];
extern uint32_t streamerSecondaryIndex;
extern volatile int streamerFlag;
extern uint32_t streamerTick;
extern uint32_t streamerLastTimestamp;
extern uint32_t streamerLastInterval;
extern volatile uint32_t streamerTotalHigh;
extern volatile uint32_t streamerTotalLow;

#else
void streamerBegin(){}
void streamerEnd(){}
bool isStreaming(){return true;}
void streamerPause(){}
void streamerResume(){}
RingBufCPP<uint16_t, 512> streamerBuffer;
volatile int streamerOverrun;
int streamerBufferMaxCounter;
uint16_t streamerSecondary[64];
uint32_t streamerSecondaryIndex;
uint32_t streamerTick;
volatile uint32_t streamerTotalHigh{0};
volatile uint32_t streamerTotalLow{0};

#endif
