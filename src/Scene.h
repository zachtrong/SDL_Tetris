#pragma once

using namespace std;

class Scene {
private:
public:
    Scene();
    virtual ~Scene();

    virtual void start() = 0;
    virtual void gameLoop() = 0;
};