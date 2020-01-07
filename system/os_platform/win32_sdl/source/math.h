// Override system math.h to declare random() method
#include <corecrt_math.h>

#ifdef _USE_MATH_DEFINES
    #include <corecrt_math_defines.h>
#endif

int random();

