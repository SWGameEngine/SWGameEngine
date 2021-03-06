//
//  Base.hpp
//  SWGameEngine
//
//  Created by James Chen on 10/26/16.
//  Copyright © 2016 James Chen. All rights reserved.
//

#ifndef base_h
#define base_h

#include <stdint.h>
#include <stddef.h>
#include <float.h>

#include <cstdlib> // for std::abs
#include <cmath>
#include <string>
#include <vector>
#include <memory>

#include "math/CCMath.h"

typedef uint8_t byte;

struct Color4
{
    Color4()
    : r(0.0f)
    , g(0.0f)
    , b(0.0f)
    , a(0.0f)
    {}

    Color4(float red, float green, float blue, float alpha)
    : r(red)
    , g(green)
    , b(blue)
    , a(alpha)
    {}

    Color4(const Color4& o)
    {
        r = o.r;
        g = o.g;
        b = o.b;
        a = o.a;
    }

    Color4 operator*(float v)
    {
        Color4 result(*this);
        result.r *= v;
        result.g *= v;
        result.b *= v;
        return result;
    }

    float r;
    float g;
    float b;
    float a;

    static Color4 White;
};

#endif /* base_h */
