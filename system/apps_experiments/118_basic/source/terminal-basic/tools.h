#ifndef TOOLS_H
#define TOOLS_H

#include "sys/cdefs.h"
#include <stdint.h>

__BEGIN_DECLS

typedef uint8_t BOOLEAN;

#ifndef TRUE
#define TRUE ((BOOLEAN)1)
#endif

#ifndef FALSE
#define FALSE ((BOOLEAN)0)
#endif

/**
 * @brief Test if character is alphabetic
 * @param 
 */
BOOLEAN tools_isAlpha(uint8_t);

BOOLEAN tools_isAlphaNum(uint8_t);

void _ftoa(float, char*);

void _dtoa(double, char*);

void readU16(uint16_t*, const uint8_t*);

void writeU16(uint16_t, uint8_t*);

void readU32(uint32_t*, const uint8_t*);

void writeU32(uint32_t, uint8_t*);

void readU64(uint64_t*, const uint8_t*);

void writeU64(uint64_t, uint8_t*);

void readR32(float*, const uint8_t*);

void writeR32(float, uint8_t*);

void writeR64(double, uint8_t*);

void readR64(double*, const uint8_t*);

#define ARRAY_SIZE(a) (sizeof(a)/sizeof(a[0]))

#define LOW_BYTE_U16(w) ((uint8_t)w)

#define HIGH_BYTE_U16(w) ((uint8_t)((w) >> 8))

__END_DECLS

#endif /* TOOLS_H */

