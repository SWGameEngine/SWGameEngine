//
//  Bitmap.hpp
//  SWGameEngine
//
//  Created by James Chen on 10/26/16.
//  Copyright © 2016 James Chen. All rights reserved.
//

#ifndef Bitmap_hpp
#define Bitmap_hpp

#include "Base.hpp"
#include "Ref.hpp"

class Bitmap : public Ref
{
public:
    Bitmap(byte* buf, int width, int height);

    byte* _buf;
    int PixelWidth;
    int PixelHeight;

    void invalidate();

    void write(byte* ptr, size_t offset, size_t len);
};

#endif /* Bitmap_hpp */
