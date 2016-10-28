//
//  Texture.cpp
//  SWGameEngine
//
//  Created by James Chen on 10/27/16.
//  Copyright © 2016 James Chen. All rights reserved.
//

#include "Texture.hpp"
#include "FileSystem.h"
#include "Image.h"

Texture::Texture(const std::string& filename, int width, int height)
{
    this->_width = width;
    this->_height = height;
    Load(filename);
}

void Texture::Load(const std::string& filename)
{
    Image img;
    img.initFromFile(filename);
    _internalBuffer = img.getData();
}

Color4 Texture::Map(float tu, float tv)
{
    // Image is not loaded yet
    if (_internalBuffer.empty())
    {
        return Color4::White;
    }
    // using a % operator to cycle/repeat the texture if needed
    int u = std::abs((int) (tu*_width) % _width);
    int v = std::abs((int) (tv*_height) % _height);

    int pos = (u + v * _width) * 4;
    byte b = _internalBuffer[pos];
    byte g = _internalBuffer[pos + 1];
    byte r = _internalBuffer[pos + 2];
    byte a = _internalBuffer[pos + 3];

    return Color4(r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f);
}
