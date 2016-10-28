#pragma once

#include "Vec2.h"


    class Size3;

    class Size2
    {
    public:
        float width;
        float height;

        Size2():
            width(0.0f), height(0.0f)
        {
        }

        Size2(float w, float h):
            width(w), height(h)
        {
        }

        Size2(const Vec2& point):
            width(point.x),
            height(point.y)
        {
        }

        Size2& operator=(const Vec2& point)
        {
            width = point.x;
            height = point.y;
            return *this;
        }

        Size2(const Size3& s);
        Size2& operator=(const Size3& other);

        Size2 operator+(const Size2& right) const
        {
            return Size2(width + right.width, height + right.height);
        }

        Size2 operator-(const Size2& right) const
        {
            return Size2(width - right.width, height - right.height);
        }

        Size2 operator*(float a) const
        {
            return Size2(width * a, height * a);
        }

        Size2 operator/(float a) const
        {
            return Size2(width / a, height / a);
        }

        inline bool operator==(const Size2& right) const
        {
            return width == right.width && height == right.height;
        }

        inline bool operator!=(const Size2& right) const
        {
            return width != right.width || height != right.height;
        }

        bool isZero() const
        {
            return width == 0.0f && height == 0.0f;
        }

        bool isOne() const
        {
            return width == 1.0f && height == 1.0f;
        }
    };

    inline Size2 operator*(const Size2& size, const Vec2& v)
    {
        return Size2(size.width * v.x, size.height * v.y);
    }

    inline Size2 operator/(const Size2& size, const Vec2& v)
    {
        return Size2(size.width / v.x, size.height / v.y);
    }
