#include "Size3.h"
#include "Size2.h"

Size3::Size3(const Size2& s):
    width(s.width),
    height(s.height),
    depth(0.0f)
{
}

Size3& Size3::operator=(const Size2& other)
{
    width = other.width;
    height = other.height;
    depth = 0.0f;
    return *this;
}
