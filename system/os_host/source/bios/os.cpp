#include "Bios.h"

namespace BIOS
{
  namespace OS
  {
    char strGlobalArgument[32];

    void SetArgument(char* argument)
    {
      _ASSERT(strlen(argument) < sizeof(strGlobalArgument)-1);
      strcpy(strGlobalArgument, argument);
    }

    char* GetArgument()
    {
      return strGlobalArgument;
    }
  }
}
