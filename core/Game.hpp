#pragma once

#include "Base.hpp"

class Device;
class Mesh;
class Camera;

class Game
{
public:
    static Game* getInstance();
    static void destroyInstance();

    void init(byte* buf, uint32_t width, uint32_t height);

    void render();

protected:

    Game();
    virtual ~Game();

    Device* _device;
    std::vector<Mesh*> _mesh;
    Camera* _mera;
};
