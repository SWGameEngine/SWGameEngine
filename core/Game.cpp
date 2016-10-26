#include "Game.hpp"

#include "Bitmap.hpp"
#include "Device.hpp"
#include "Mesh.hpp"
#include "Camera.hpp"

static Game* __instance = nullptr;

Game* Game::getInstance()
{
    if (__instance == nullptr)
    {
        __instance = new Game();
    }
    return __instance;
}

void Game::destroyInstance()
{
    delete __instance;
    __instance = nullptr;
}

Game::Game()
{
    _mesh.push_back(new Mesh("Cube", 8));
    _mera = new Camera();
}

Game::~Game()
{
    
}

void Game::onInit()
{
    // Choose the back buffer resolution here
    Bitmap* bmp = new Bitmap(640, 480);

    _device = new Device(bmp);

    // Our XAML Image control
//    frontBuffer.Source = bmp;

    _mesh[0]->Vertices[0] = Vec3(-1, 1, 1);
    _mesh[0]->Vertices[1] = Vec3(1, 1, 1);
    _mesh[0]->Vertices[2] = Vec3(-1, -1, 1);
    _mesh[0]->Vertices[3] = Vec3(-1, -1, -1);
    _mesh[0]->Vertices[4] = Vec3(-1, 1, -1);
    _mesh[0]->Vertices[5] = Vec3(1, 1, -1);
    _mesh[0]->Vertices[6] = Vec3(1, -1, 1);
    _mesh[0]->Vertices[7] = Vec3(1, -1, -1);

    _mera->Position = Vec3(0, 0, 10.0f);
    _mera->Target = Vec3::ZERO;

    // Registering to the XAML rendering loop
//    CompositionTarget.Rendering += CompositionTarget_Rendering;
}

// Rendering loop handler
void Game::render()
{
    _device->Clear(0, 0, 0, 255);

    // rotating slightly the cube during each frame rendered
    _mesh[0]->Rotation = Vec3(_mesh[0]->Rotation.x + 0.01f, _mesh[0]->Rotation.y + 0.01f, _mesh[0]->Rotation.z);

    // Doing the various matrix operations
    _device->Render(_mera, _mesh);
    // Flushing the back buffer into the front buffer
    _device->Present();
}
