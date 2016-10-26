//
//  Device.hpp
//  SWGameEngine
//
//  Created by James Chen on 10/26/16.
//  Copyright © 2016 James Chen. All rights reserved.
//

#ifndef Device_hpp
#define Device_hpp

#include "Base.hpp"

class Bitmap;
class Camera;
class Mesh;

class Device
{
public:
    Device(Bitmap* bmp);
    virtual ~Device();

    // This method is called to clear the back buffer with a specific color
    void Clear(byte r, byte g, byte b, byte a);

    // Once everything is ready, we can flush the back buffer
    // into the front buffer.
    void Present();

    // Called to put a pixel on screen at a specific X,Y coordinates
    void PutPixel(int x, int y, Color4 color);

    // Project takes some 3D coordinates and transform them
    // in 2D coordinates using the transformation matrix
    Vec2 Project(const Vec3& coord, const Mat4& transMat);

    void DrawLine(Vec2 point0, Vec2 point1);
    // DrawPoint calls PutPixel but does the clipping operation before
    void DrawPoint(const Vec3& point);

    // The main method of the engine that re-compute each vertex projection
    // during each frame
    void Render(Camera* camera, const std::vector<Mesh*>& meshes);

private:
    std::vector<byte> _backBuffer;
    Bitmap* _bmp;
};

#endif /* Device_hpp */
