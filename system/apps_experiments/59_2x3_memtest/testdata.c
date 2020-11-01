#include <stdint.h>

//__attribute__((__section__(".rodata")))
const uint32_t testdata[1024] = {[0 ... 1023] = 0xF00F0000};
