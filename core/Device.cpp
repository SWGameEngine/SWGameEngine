//
//  Device.cpp
//  SWGameEngine
//
//  Created by James Chen on 10/26/16.
//  Copyright © 2016 James Chen. All rights reserved.
//

#include "Device.hpp"

#include "Base.hpp"
#include "Bitmap.hpp"
#include "Camera.hpp"
#include "Mesh.hpp"



Device::Device(Bitmap* bmp)
{
    _bmp = bmp;
    _bmp->addRef();
    // the back buffer size is equal to the number of pixels to draw
    // on screen (width*height) * 4 (R,G,B & Alpha values).
    _backBuffer.resize(bmp->PixelWidth * bmp->PixelHeight * 4);
}

Device::~Device()
{
    _bmp->release();
}

// This method is called to clear the back buffer with a specific color
void Device::Clear(byte r, byte g, byte b, byte a) {
    for (int index = 0; index < _backBuffer.size(); index += 4)
    {
        // BGRA is used by Windows instead by RGBA in HTML5
        _backBuffer[index] = a;
        _backBuffer[index + 1] = r;
        _backBuffer[index + 2] = g;
        _backBuffer[index + 3] = b;
    }
}

// Once everything is ready, we can flush the back buffer
// into the front buffer.
void Device::Present()
{
    // writing our byte[] back buffer into our Bitmap stream
    _bmp->write(_backBuffer.data(), 0, _backBuffer.size());
    // request a redraw of the entire bitmap
    _bmp->invalidate();
}

// Called to put a pixel on screen at a specific X,Y coordinates
void Device::PutPixel(int x, int y, Color4 color)
{
    // As we have a 1-D Array for our back buffer
    // we need to know the equivalent cell in 1-D based
    // on the 2D coordinates on screen
    int index = (x + y * _bmp->PixelWidth) * 4;

    _backBuffer[index] = (byte)(color.a * 255);
    _backBuffer[index + 1] = (byte)(color.r * 255);
    _backBuffer[index + 2] = (byte)(color.g * 255);
    _backBuffer[index + 3] = (byte)(color.b * 255);
}

// Project takes some 3D coordinates and transform them
// in 2D coordinates using the transformation matrix
Vec2 Device::Project(const Vec3& coord, const Mat4& transMat)
{
    // transforming the coordinates
    Vec4 point = Vec4(coord.x, coord.y, coord.z, 1);
    transMat.transformVector(&point);

    // The transformed coordinates will be based on coordinate system
    // starting on the center of the screen. But drawing on screen normally starts
    // from top left. We then need to transform them again to have x:0, y:0 on top left.
    float x = point.x/point.w * _bmp->PixelWidth + _bmp->PixelWidth / 2.0f;
    float y = point.y/point.w * _bmp->PixelHeight + _bmp->PixelHeight / 2.0f;
    return Vec2(x, y);
//    return point;
}

void Device::DrawLine(const Vec2& point0, const Vec2& point1)
{
    int x0 = (int)point0.x;
    int y0 = (int)point0.y;
    int x1 = (int)point1.x;
    int y1 = (int)point1.y;

    int dx = std::abs(x1 - x0);
    int dy = std::abs(y1 - y0);
    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;
    int err = dx - dy;

    while (true) {
        DrawPoint(Vec2(x0, y0));

        if ((x0 == x1) && (y0 == y1)) break;
        int e2 = 2 * err;
        if (e2 > -dy) { err -= dy; x0 += sx; }
        if (e2 < dx) { err += dx; y0 += sy; }
    }
}

// DrawPoint calls PutPixel but does the clipping operation before
void Device::DrawPoint(const Vec2& point)
{
    // Clipping what's visible on screen
    if (point.x >= 0 && point.y >= 0 && point.x < _bmp->PixelWidth && point.y < _bmp->PixelHeight)
    {
        // Drawing a yellow point
        PutPixel((int)point.x, (int)point.y, Color4(1.0f, 1.0f, 0.0f, 1.0f));
    }
}

// The main method of the engine that re-compute each vertex projection
// during each frame
void Device::Render(Camera* camera, const std::vector<Mesh*>& meshes)
{
    // To understand this part, please read the prerequisites resources
    Mat4 viewMat4;
    Mat4::createLookAt(camera->Position, camera->Target, Vec3::UNIT_Y, &viewMat4);

    Mat4 projectionMat4;
    Mat4::createPerspective(MATH_RAD_TO_DEG(0.78f), (float)_bmp->PixelWidth / _bmp->PixelHeight, 0.01f, 1.0f, &projectionMat4);

    for (auto&& mesh : meshes)
    {
        // Beware to apply rotation before translation
        Mat4 worldMat4;
//        Mat4::createRotation(const Quaternion &quat, &worldMat4);

//        .RotationYawPitchRoll(mesh->Rotation.Y,
//                                                      mesh->Rotation.X, mesh->Rotation.Z) *

        // https://msdn.microsoft.com/en-us/library/microsoft.windowsmobile.directx.matrix.rotationyawpitchroll(v=vs.90).aspx
        // The order of transformations is first roll, then pitch, then yaw. Relative to the object's local coordinate axis, this is equivalent to rotation around the z-axis, followed by rotation around the x-axis, followed by rotation around the y-axis.

        worldMat4.scale(mesh->scale.x, mesh->scale.y, mesh->scale.z);
        //FIXME: Mat4 needs a create method to create Matrix with rotation x, y, z
        worldMat4.rotateZ(mesh->Rotation.z); // Roll
        worldMat4.rotateX(mesh->Rotation.x); // Pitch
        worldMat4.rotateY(mesh->Rotation.y); // Yaw

        worldMat4.translate(mesh->Position);

        Mat4 transformMatrix = projectionMat4 * viewMat4 * worldMat4;

        // First, we project the 3D coordinates into the 2D space
//        for (size_t i = 0, len = mesh->Vertices.size() - 1; i < len; i++)
//        {
//            auto point0 = Project(mesh->Vertices[i], transformMatrix);
//            auto point1 = Project(mesh->Vertices[i + 1], transformMatrix);
//            DrawLine(point0, point1);
//        }

        for (auto&& face : mesh->Faces)
        {
            auto vertexA = mesh->Vertices[face.A];
            auto vertexB = mesh->Vertices[face.B];
            auto vertexC = mesh->Vertices[face.C];

            auto pixelA = Project(vertexA, transformMatrix);
            auto pixelB = Project(vertexB, transformMatrix);
            auto pixelC = Project(vertexC, transformMatrix);

            DrawLine(pixelA, pixelB);
            DrawLine(pixelB, pixelC);
            DrawLine(pixelC, pixelA);
        }
    }
}
