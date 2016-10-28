//
//  Texture.hpp
//  SWGameEngine
//
//  Created by James Chen on 10/27/16.
//  Copyright © 2016 James Chen. All rights reserved.
//

#ifndef Texture_hpp
#define Texture_hpp

#include "Base.hpp"

class Texture
{
public:
    // Working with a fix sized texture (512x512, 1024x1024, etc.).
    Texture(const std::string& filename, int width, int height);

    void Load(const std::string& filename);

    // Takes the U & V coordinates exported by Blender
    // and return the corresponding pixel color in the texture
    Color4 Map(float tu, float tv);

private:
    std::vector<uint8_t> _internalBuffer;
    int _width;
    int _height;
};

#endif /* Texture_hpp */
