#include "library.h"
#include "../source/codecs/codecs.h"

int length = 0; 
uint16_t data[1024];

bool Load(const char* filename)
{
  length = 0;
  FILE* f = fopen(filename, "r");
  if (!f)
    return false;

  char buf[8];
  int bufi = 0;
  while (!feof(f))
  {
     uint8_t c = fgetc(f);
     if (c >= '0' && c <= '9')
     {
         if (bufi<7)
             buf[bufi++] = c;
     } else
     {
         if (bufi>0)
         {
             buf[bufi] = 0;
             data[length++] = atoi(buf);
             bufi = 0;
         }
     }
  }

  fclose(f);
  return true;
}

void Decode()
{
  for (int i=0; mCodecs[i]; i++)
  {
    if (mCodecs[i]->Decode(data, length))
    {
      char info[1024];
      mCodecs[i]->GetShortInfo(info);
      printf("%s: %s\n", mCodecs[i]->Id(), info);
      return;
    }
  }
  printf("Unable to decode!\n");
}

int main()
{
  printf("Running tests\n");
  Load("../logs/rf_oregon1.sig");
  Decode();
  Load("../logs/ir_volumeup.sig");
  Decode();
  Load("../logs/rf_unk_ba1.sig");
  Decode();
  Load("../logs/rf_unk_ba2.sig");
  Decode();
  Load("../logs/rf_unk_ba3.sig");
  Decode();
  Load("../logs/rf_unk_ba4.sig");
  Decode();
  Load("../logs/rf_unk_ba5.sig");
  Decode();
  Load("../logs/rf_unk_sd1.sig");
  Decode();
  Load("../logs/rf_unk_sd2.sig");
  Decode();
  Load("../logs/rf_unk_sd3.sig");
  Decode();
  return 0;
}