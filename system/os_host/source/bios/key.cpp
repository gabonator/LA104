#include "Bios.h"
#include "imports.h"

namespace BIOS {
  namespace KEY {
    EKey GetKey() 
    {
      char c = GetLastChar();
      if (!c)
        return None;

      switch (c)
      {
        case '+': return Right;
        case '-': return Left;
        case '<': return Up;
        case '>': return Down;
        case '1': return F1;
        case '2': return F2;
        case '3': return F3;
        case '4': return F4;
        default: return None;
      }
    }
  }
}
