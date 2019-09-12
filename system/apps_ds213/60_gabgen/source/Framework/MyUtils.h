#include <library.h>

namespace CUtils
{

/*static*/ char* FormatFrequency( float fF, int nChars=8 );
/*static*/ char* FormatTime( float fT, int nChars=8 );
template <typename T, typename TA, typename TB> 
T Clamp(T in, TA minval, TB maxval) 
{
  if (in > (T)maxval)
    return (T)maxval;
  if (in < (T)minval)
    return (T)minval;
  return in;
}

}