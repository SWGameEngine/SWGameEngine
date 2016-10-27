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
#include "Mesh.hpp"

class Bitmap;
class Camera;

struct ScanLineData
{
    int currentY = 0;
    float ndotla = 0.0f;
    float ndotlb = 0.0f;
    float ndotlc = 0.0f;
    float ndotld = 0.0f;
};

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
    void PutPixel(int x, int y, float z, Color4 color);

    // Project takes some 3D coordinates and transform them
    // in 2D coordinates using the transformation matrix
    // It also transform the same coordinates and the norma to the vertex
    // in the 3D world
    Vertex Project(const Vertex& vertex, const Mat4& transMat, const Mat4& world);

    void DrawLine(const Vec2& point0, const Vec2& point1);
    // DrawPoint calls PutPixel but does the clipping operation before
    void DrawPoint(const Vec3& point, Color4 color);

    void ProcessScanLine(ScanLineData data, Vertex va, Vertex vb, Vertex vc, Vertex vd, Color4 color);
    float ComputeNDotL(Vec3 vertex, Vec3 normal, Vec3 lightPosition);
    void DrawTriangle(Vertex v1, Vertex v2, Vertex v3, Color4 color);

    // The main method of the engine that re-compute each vertex projection
    // during each frame
    void Render(Camera* camera, const std::vector<Mesh*>& meshes);

    std::vector<Mesh*> LoadJSONFile(const std::string& fileName);

private:
    std::vector<byte> _backBuffer;
    std::vector<float> _depthBuffer;
    Bitmap* _bmp;
};

#endif /* Device_hpp */
