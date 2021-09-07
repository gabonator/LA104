#pragma once
#include "Print.h"

class Printable
{
    virtual size_t printTo(Print& p) const { return 0; }
};
