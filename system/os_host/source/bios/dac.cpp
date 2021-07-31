#include "Bios.h"

#if defined(DS203) || defined(DS203HD)
#include "ds203/dac.cpp"
#endif

#if defined(DS213)
#include "ds213/dac.cpp"
#endif