#ifdef EMSCRIPTEN

#include <library.h>
#include "SPI.h"

void CSPI::begin() {}
void CSPI::end() {}
void CSPI::setClockDivider(int clockDivider) {}
void CSPI::dmaSendAsync(uint8_t* data, int len) {}

CSPI SPI;
#endif
