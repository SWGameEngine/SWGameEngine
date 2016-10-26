//
//  Mesh.hpp
//  SWGameEngine
//
//  Created by James Chen on 10/26/16.
//  Copyright © 2016 James Chen. All rights reserved.
//

#ifndef Mesh_hpp
#define Mesh_hpp

#include "Base.hpp"

class Mesh
{
public:
    std::string Name;
    std::vector<Vec3> Vertices;
    Vec3 Position;
    Vec3 Rotation;

    Mesh(const std::string& name, int verticesCount)
    {
        Vertices.resize(verticesCount);
        Name = name;
    }
};

#endif /* Mesh_hpp */
