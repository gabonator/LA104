#pragma once

#include <stdint.h>
#include <cstring>

// simple two pointer ring buffer. Can hold MaxElements-1 elements, when
// empty when first == last
// producer is interrupt
// consumer is main loop which can be interrupted any time

template <typename Type, size_t MaxElements>
class RingBuf
{
    Type buffer[MaxElements];
    Type* wrap;
    Type* first;
    Type* last;
public:
    RingBuf()
    {
        wrap = buffer + MaxElements;
        first = buffer;
        last = buffer;
    }
    int size()
    {
        if (last >= first)
            return static_cast<int>(last - first);
        else
            return static_cast<int>(last - first + MaxElements);
    }
    int avail()
    {
        return static_cast<int>(MaxElements) - size() - 1;
    }
    void push(Type t)
    {
        //assert(avail() > 0);
        *last++ = t;
        if (last == wrap)
            last = buffer;
    }
    Type pull()
    {
        //assert(size() > 0);
        Type aux = *first++;
        if (first == wrap)
            first = buffer;
        return aux;
    }
};

