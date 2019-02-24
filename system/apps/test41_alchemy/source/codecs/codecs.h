#include "codec.h"
#include "oregon.h"
#include "samsung.h"
#include "tvremote1.h"
#include "weather1.h"

CCodecOregon mCodecOregon;
CCodecSamsung mCodecSamsung;
CCodecTvRemote1 mCodecTvRemote1;
//CCodecWeather1 mCodecWeather1;

CCodec* mCodecs[] = {&mCodecOregon, &mCodecSamsung, &mCodecTvRemote1, /*&mCodecWeather1,*/ nullptr};