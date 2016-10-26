//
//  Bitmap.cpp
//  SWGameEngine
//
//  Created by James Chen on 10/26/16.
//  Copyright © 2016 James Chen. All rights reserved.
//

#include "Bitmap.hpp"

Bitmap::Bitmap(byte* buf, int width, int height)
: _buf(buf)
, PixelWidth(width)
, PixelHeight(height)
{

}

void Bitmap::invalidate()
{

}

void Bitmap::write(byte* ptr, size_t offset, size_t len)
{
    memcpy(_buf, ptr + offset, len);
}
