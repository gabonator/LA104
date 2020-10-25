#include "Bios.h"
#include "imports.h"

#ifdef DS203
extern uint32_t gKeyMask;
#endif

namespace BIOS {
  namespace KEY {

#ifdef DS203
    class CKeyProcessor
    {
    	char keys{0};
    	int32_t last;
    	int delay{301};

    public:
    	CKeyProcessor()
    	{
    		last = BIOS::SYS::GetTick();
    	}
    	void operator <<(const char& in)
    	{
    		if (keys == 0)
    			delay = 301;
    		keys = in;
    	}
    	void operator >>(char& out)
    	{
    		int32_t now = BIOS::SYS::GetTick();
    		if (now-last > delay && keys)
    		{
    			out = keys;
    			last = now;
    			if ( delay == 301 )
    				delay = 300;
    			else if ( delay == 300 )
    				delay = 100;
    			else if (delay > 10)
    				delay -= 10;
    		}
    		else
    			out = 0;
    	}
    };
/*
    class CKeyProcessor
    {
    	char keys{0};

    public:
    	void operator <<(const char& in)
    	{
    	  keys = in;
    	}
    	void operator >>(char& out)
        {
          out = keys;
        }
    };
*/
    CKeyProcessor processor;
#endif

    EKey GetKey() 
    {
#ifdef DS203
      char lastChar = 0;
      if (gKeyMask & KeyUp)
        lastChar = '>';
      if (gKeyMask & KeyDown)
        lastChar = '<';
      if (gKeyMask & KeyRight)
        lastChar = '+';
      if (gKeyMask & KeyLeft)
        lastChar = '-';
      if (gKeyMask & KeyF1)
        lastChar = '1';
      if (gKeyMask & KeyF2)
        lastChar = '2';
      if (gKeyMask & KeyF3)
        lastChar = '3';
      if (gKeyMask & KeyF4)
        lastChar = '4';

      processor << lastChar;

      char c;
      processor >> c;
#else
      char c = GetLastChar();
#endif

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
