#include "Bios.h"
#include "hal.h"

bool sdl_running();
void sdl_loop();
int sdl_lastKey();

namespace BIOS {
  namespace KEY {
    bool KeyPressed()
      {
          return false;
      }
      
    EKey GetKey() 
    {
        if (!gHal->IsRunning())
            return Escape;
        
        char c = gHal->GetKey();
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
