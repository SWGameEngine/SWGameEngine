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
#include "files/FileSystem.h"
#include "external/rapidjson/rapidjson.h"
#include "external/rapidjson/document.h"

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
Vec3 Device::Project(const Vec3& coord, const Mat4& transMat)
{
    // transforming the coordinates
    Vec4 point = Vec4(coord.x, coord.y, coord.z, 1);
    transMat.transformVector(&point);

    // The transformed coordinates will be based on coordinate system
    // starting on the center of the screen. But drawing on screen normally starts
    // from top left. We then need to transform them again to have x:0, y:0 on top left.
    float x = point.x/point.w * _bmp->PixelWidth + _bmp->PixelWidth / 2.0f;
    float y = -point.y/point.w * _bmp->PixelHeight + _bmp->PixelHeight / 2.0f;
    return Vec3(x, y, point.z/point.w);
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
        DrawPoint(Vec2(x0, y0), Color4(1, 1, 0, 1));

        if ((x0 == x1) && (y0 == y1)) break;
        int e2 = 2 * err;
        if (e2 > -dy) { err -= dy; x0 += sx; }
        if (e2 < dx) { err += dx; y0 += sy; }
    }
}

// DrawPoint calls PutPixel but does the clipping operation before
void Device::DrawPoint(const Vec2& point, Color4 color)
{
    // Clipping what's visible on screen
    if (point.x >= 0 && point.y >= 0 && point.x < _bmp->PixelWidth && point.y < _bmp->PixelHeight)
    {
        // Drawing a yellow point
        PutPixel((int)point.x, (int)point.y, color);
    }
}

// Clamping values to keep them between 0 and 1
static float Clamp(float value, float min = 0, float max = 1)
{
    return std::max(min, std::min(value, max));
}

// Interpolating the value between 2 vertices
// min is the starting point, max the ending point
// and gradient the % between the 2 points
static float Interpolate(float min, float max, float gradient)
{
    return min + (max - min) * Clamp(gradient);
}

// drawing line between 2 points from left to right
// papb -> pcpd
// pa, pb, pc, pd must then be sorted before
void Device::ProcessScanLine(int y, Vec3 pa, Vec3 pb, Vec3 pc, Vec3 pd, Color4 color)
{
    // Thanks to current Y, we can compute the gradient to compute others values like
    // the starting X (sx) and ending X (ex) to draw between
    // if pa.y == pb.y or pc.y == pd.y, gradient is forced to 1
    float gradient1 = pa.y != pb.y ? (y - pa.y) / (pb.y - pa.y) : 1;
    float gradient2 = pc.y != pd.y ? (y - pc.y) / (pd.y - pc.y) : 1;

    int sx = (int)Interpolate(pa.x, pb.x, gradient1);
    int ex = (int)Interpolate(pc.x, pd.x, gradient2);

    // drawing a line from left (sx) to right (ex)
    for (int x = sx; x < ex; x++)
    {
        DrawPoint(Vec2(x, y), color);
    }
}

void Device::DrawTriangle(Vec3 p1, Vec3 p2, Vec3 p3, Color4 color)
{
    // Sorting the points in order to always have this order on screen p1, p2 & p3
    // with p1 always up (thus having the Y the lowest possible to be near the top screen)
    // then p2 between p1 & p3
    if (p1.y > p2.y)
    {
        auto temp = p2;
        p2 = p1;
        p1 = temp;
    }

    if (p2.y > p3.y)
    {
        auto temp = p2;
        p2 = p3;
        p3 = temp;
    }

    if (p1.y > p2.y)
    {
        auto temp = p2;
        p2 = p1;
        p1 = temp;
    }

    // inverse slopes
    float dP1P2, dP1P3;

    // http://en.wikipedia.org/wiki/Slope
    // Computing inverse slopes
    if (p2.y - p1.y > 0)
        dP1P2 = (p2.x - p1.x) / (p2.y - p1.y);
    else
        dP1P2 = 0;

    if (p3.y - p1.y > 0)
        dP1P3 = (p3.x - p1.x) / (p3.y - p1.y);
    else
        dP1P3 = 0;

    // First case where triangles are like that:
    // P1
    // -
    // --
    // - -
    // -  -
    // -   - P2
    // -  -
    // - -
    // -
    // P3
    if (dP1P2 > dP1P3)
    {
        for (int y = (int)p1.y; y <= (int)p3.y; y++)
        {
            if (y < p2.y)
            {
                ProcessScanLine(y, p1, p3, p1, p2, color);
            }
            else
            {
                ProcessScanLine(y, p1, p3, p2, p3, color);
            }
        }
    }
    // First case where triangles are like that:
    //       P1
    //        -
    //       --
    //      - -
    //     -  -
    // P2 -   -
    //     -  -
    //      - -
    //        -
    //       P3
    else
    {
        for (int y = (int)p1.y; y <= (int)p3.y; y++)
        {
            if (y < p2.y)
            {
                ProcessScanLine(y, p1, p2, p1, p3, color);
            }
            else
            {
                ProcessScanLine(y, p2, p3, p1, p3, color);
            }
        }
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

        int faceIndex = 0;
        for (auto&& face : mesh->Faces)
        {
            auto vertexA = mesh->Vertices[face.A];
            auto vertexB = mesh->Vertices[face.B];
            auto vertexC = mesh->Vertices[face.C];

            auto pixelA = Project(vertexA, transformMatrix);
            auto pixelB = Project(vertexB, transformMatrix);
            auto pixelC = Project(vertexC, transformMatrix);

//            DrawLine(pixelA, pixelB);
//            DrawLine(pixelB, pixelC);
//            DrawLine(pixelC, pixelA);

            auto color = 0.25f + (faceIndex % mesh->Faces.size()) * 0.75f / mesh->Faces.size();
            DrawTriangle(pixelA, pixelB, pixelC, Color4(color, color, color, 1));
            ++faceIndex;
        }
    }
}

std::vector<Mesh*> Device::LoadJSONFile(const std::string& fileName)
{
    std::vector<Mesh*> meshes;

    std::vector<uint8_t> modelData;
    FileSystem system;
    system.loadFile("models/monkey.babylon", modelData);

    rapidjson::Document jsonObject;
    jsonObject.Parse((char*)modelData.data());

    for (int meshIndex = 0, len = jsonObject["meshes"].Size(); meshIndex < len; meshIndex++)
    {
        const auto& meshObj = jsonObject["meshes"][meshIndex];
        const auto& verticesArray = meshObj["vertices"];
        // Faces
        const auto& indicesArray = meshObj["indices"];

        int uvCount = meshObj["uvCount"].GetInt();
        int verticesStep = 1;

        // Depending of the number of texture's coordinates per vertex
        // we're jumping in the vertices array  by 6, 8 & 10 windows frame
        switch (uvCount)
        {
            case 0:
                verticesStep = 6;
                break;
            case 1:
                verticesStep = 8;
                break;
            case 2:
                verticesStep = 10;
                break;
        }

        // the number of interesting vertices information for us
        int verticesCount = verticesArray.Size() / verticesStep;
        // number of faces is logically the size of the array divided by 3 (A, B, C)
        int facesCount = indicesArray.Size() / 3;
        auto mesh = new Mesh(meshObj["name"].GetString(), verticesCount, facesCount);

        // Filling the Vertices array of our mesh first
        for (int index = 0; index < verticesCount; index++)
        {
            float x = (float)verticesArray[index * verticesStep].GetDouble();
            float y = (float)verticesArray[index * verticesStep + 1].GetDouble();
            float z = (float)verticesArray[index * verticesStep + 2].GetDouble();
            mesh->Vertices[index] = Vec3(x, y, z);
        }

        // Then filling the Faces array
        for (int index = 0; index < facesCount; index++)
        {
            int a = (int)indicesArray[index * 3].GetInt();
            int b = (int)indicesArray[index * 3 + 1].GetInt();
            int c = (int)indicesArray[index * 3 + 2].GetInt();
            mesh->Faces[index] = { a, b, c };
        }

        // Getting the position you've set in Blender
        const auto& position = meshObj["position"];
        mesh->Position = Vec3((float)position[0].GetDouble(), (float)position[1].GetDouble(), (float)position[2].GetDouble());
        meshes.push_back(mesh);
    }
    return meshes;
}
