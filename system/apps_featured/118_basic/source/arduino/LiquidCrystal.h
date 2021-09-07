#include <assert.h>

class LiquidCrystal
{
public:
void clear() { assert(0); }
void setCursor(int, int) { assert(0); }
void write(char c){ assert(0); }
void noCursor(){ assert(0); }
void cursor(){ assert(0); }
};
