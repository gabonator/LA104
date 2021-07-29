#include "Framework.h"

class CCoreOscilloscope
{
public:
	static void ConfigureAdc();
	static void ConfigureTrigger();
	//static void UpdateConstants();
	// mask
	static void GetMaskAt(int nX, int** pLow, int** pHigh);
	static void GetMaskStats(int** nPass, int** nFail);
};
