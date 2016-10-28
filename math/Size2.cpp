#include "Size2.h"
#include "Size3.h"

    Size2::Size2(const Size3& s):
        width(s.width),
        height(s.height)
    {
    }

    Size2& Size2::operator=(const Size3& other)
    {
        width = other.width;
        height = other.height;
        return *this;
    }
