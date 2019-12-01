#include "wasmhal.h"
CHal* gHal{nullptr};

extern "C" void appInit();
extern "C" bool appLoop();
extern "C" void appFinish();
//extern "C" uint32_t appAnalyseResultPtr();
//extern "C" bool appAnalyse();
//extern "C" void appSetDataCount(uint32_t count);
//extern "C" uint32_t appGetDataPtr();

//extern "C" void* appVideoBuffer();
//extern "C" bool appVideoBufferChanged();

//uint8_t pixels[BIOS::LCD::Width * BIOS::LCD::Height*4];
//bool pixelsChanged{false};

void mainInit();
int mainLoop();
void mainFinish();

CWasmHal hal;

//bool appVideoBufferChanged()
//{
//  if (!pixelsChanged)
//    return false;
//
//  pixelsChanged = false;
//  return true;
//}

//void* appVideoBuffer()
//{
//  return pixels;
//}

void appInit()
{
  gHal = &hal;
  mainInit();
}

bool appLoop()
{
  return mainLoop();
}

void appFinish()
{
  mainFinish();
  gHal = nullptr;
}
/*
char* appBIOS_OS_GetArgument()
{
  return BIOS::OS::GetArgument();
}
*/
/*
class CStorage
{
public:
    int mSignalLength{0};
    uint16_t mSignalData[450];
};
#include "../../../apps/test41_alchemy/source/codecs/codec.h"

extern CStorage mStorage;
extern CCodec* mCodecs[];
//#include "../../../apps/test41_alchemy/source/settings.h"

uint32_t appGetDataPtr()
{
  return (uint32_t)mStorage.mSignalData;
}

void appSetDataCount(uint32_t count)
{
  mStorage.mSignalLength = count;
}

uint32_t appAnalyseResultPtr()
{
  static char buffer[512];
  return (uint32_t)buffer;
}

bool appAnalyse()
{
    char* p = (char*)appAnalyseResultPtr();
    for (int i=0; mCodecs[i]; i++)
    {
        CCodec* pCodec = mCodecs[i];
        if (pCodec->Decode(mStorage.mSignalData, mStorage.mSignalLength))
        {
            strcpy(p, pCodec->Id());
            pCodec->GetShortInfo(p+strlen(p));
            return true;
        }
    } 
    return false;
}
*/