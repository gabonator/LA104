#include "codec.h"
#include "oregon.h"
#include "samsung.h"
#include "tvremote1.h"
#include "weather1.h"
#include "unknown1.h"
#include "unknown2.h"
#include "unknown3.h"
#include "unknown4.h"

CCodecOregon mCodecOregon;
CCodecSamsung mCodecSamsung;
CCodecTvRemote1 mCodecTvRemote1;
//CCodecWeather1 mCodecWeather1;
CCodecUnknown1 mCodecUnknown1;
CCodecUnknown2 mCodecUnknown2;
CCodecUnknown3 mCodecUnknown3;
CCodecUnknown4 mCodecUnknown4;

CCodec* mCodecs[] = {&mCodecOregon, &mCodecSamsung, &mCodecTvRemote1, 
/*&mCodecWeather1,*/ &mCodecUnknown1, &mCodecUnknown2, &mCodecUnknown3, 
&mCodecUnknown4, nullptr};