#pragma once

#include <vector>

class Device;
class Mesh;
class Camera;

class Game
{
public:
    static Game* getInstance();
    static void destroyInstance();

    Game();
    virtual ~Game();
    
    void onInit();
    void onDestroy();

    void render();

private:
    Device* _device;

    std::vector<Mesh*> _mesh;
    Camera* _mera;
};
