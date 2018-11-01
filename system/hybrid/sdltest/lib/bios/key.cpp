#include "Bios.h"

bool sdl_running();
void sdl_loop();
int sdl_lastKey();

namespace BIOS {
  namespace KEY {
    EKey GetKey() 
    {
      sdl_loop();
        
        char c = 0;
        switch (sdl_lastKey())
        {
            case SDL_SCANCODE_LEFT: c = '-'; break;
            case SDL_SCANCODE_RIGHT: c = '+'; break;
            case SDL_SCANCODE_UP: c = '<'; break;
            case SDL_SCANCODE_DOWN: c = '>'; break;
            case SDL_SCANCODE_RETURN: c = '1'; break;
        }
        
        if (!sdl_running())
            return Escape;
        
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
