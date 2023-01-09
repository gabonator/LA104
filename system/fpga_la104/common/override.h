#undef VL_PRINTF 
#define VL_PRINTF Verilator::PrintSim 

namespace Verilator {
    int PrintSim(const char *format, ...);
}
