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

}

Game::~Game()
{
    for (auto&& mesh : _meshes)
    {
        delete mesh;
    }
    delete _mera;
    delete _device;
}

void Game::init(byte* buf, uint32_t width, uint32_t height)
{
    _mera = new Camera();
    _mera->Position = Vec3(0, 0, -400.0f);
    _mera->Target = Vec3::ZERO;

    // Choose the back buffer resolution here
    Bitmap* bmp = new Bitmap(buf, width, height);
    _device = new Device(bmp);
    bmp->release();

//    auto mesh = new Mesh("Cube", 8, 12);
//
//    mesh->Vertices[0] = Vec3(-1, 1, 1);
//    mesh->Vertices[1] = Vec3(1, 1, 1);
//    mesh->Vertices[2] = Vec3(-1, -1, 1);
//    mesh->Vertices[3] = Vec3(1, -1, 1);
//    mesh->Vertices[4] = Vec3(-1, 1, -1);
//    mesh->Vertices[5] = Vec3(1, 1, -1);
//    mesh->Vertices[6] = Vec3(1, -1, -1);
//    mesh->Vertices[7] = Vec3(-1, -1, -1);
//
//    mesh->Faces[0] = { 0, 1, 2 };
//    mesh->Faces[1] = { 1, 2, 3 };
//    mesh->Faces[2] = { 1, 3, 6 };
//    mesh->Faces[3] = { 1, 5, 6 };
//    mesh->Faces[4] = { 0, 1, 4 };
//    mesh->Faces[5] = { 1, 4, 5 };
//
//    mesh->Faces[6] = { 2, 3, 7 };
//    mesh->Faces[7] = { 3, 6, 7 };
//    mesh->Faces[8] = { 0, 2, 7 };
//    mesh->Faces[9] = { 0, 4, 7 };
//    mesh->Faces[10] = { 4, 5, 6 };
//    mesh->Faces[11] = { 4, 6, 7 };
//    _meshes.push_back(mesh);

    _meshes = _device->LoadJSONFile("models/monkey.babylon");
}

// Rendering loop handler
void Game::render()
{
    _device->Clear(0, 0, 0, 255);

    // rotating slightly the cube during each frame rendered
    _meshes[0]->Rotation = Vec3(_meshes[0]->Rotation.x, _meshes[0]->Rotation.y + 0.01f, _meshes[0]->Rotation.z);

    _meshes[0]->scale = {50, 50, 50};
//    _meshes[0]->scale = {0.5, 0.5, 0.5};
    _meshes[0]->Position = {0, 0, 0};
    // Doing the various matrix operations
    _device->Render(_mera, _meshes);
    // Flushing the back buffer into the front buffer
    _device->Present();
}
