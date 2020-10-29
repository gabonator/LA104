#define ADC_FIFO_SIZE 256
#define ADC_FIFO_HALFSIZE (ADC_FIFO_SIZE/2)

void adcConfigure();
void streamerInit();
void streamerDeinit();
void streamerStart();
void streamerStop();
