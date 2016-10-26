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

struct Face
{
    int A;
    int B;
    int C;
};

class Mesh
{
public:
    std::string Name;
    std::vector<Vec3> Vertices;
    std::vector<Face> Faces;
    Vec3 Position;
    Vec3 Rotation;
    Vec3 scale;

    Mesh(const std::string& name, int verticesCount, int facesCount)
    {
        Vertices.resize(verticesCount);
        Faces.resize(facesCount);
        Name = name;
        scale.set(1.0f, 1.0f, 1.0f);
    }
};

#endif /* Mesh_hpp */
