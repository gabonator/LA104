#include "wasmhal.h"
CHal* gHal{nullptr};

extern "C" void appInit();
extern "C" void appLoop();
extern "C" void appFinish();
extern "C" uint32_t appAnalyseResultPtr();
extern "C" bool appAnalyse();
extern "C" void appSetDataCount(uint32_t count);
extern "C" uint32_t appGetDataPtr();

void mainInit();
bool mainLoop();
void mainFinish();

CWasmHal hal;

void appInit()
{
  gHal = &hal;
  mainInit();
}

void appLoop()
{
  mainLoop();
}

void appFinish()
{
  mainFinish();
  gHal = nullptr;
}

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