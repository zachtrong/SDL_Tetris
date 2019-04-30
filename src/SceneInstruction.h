#pragma once
#include "Scene.h"

using namespace std;

class SceneInstruction : public Scene {
private:
public:
    SceneInstruction();
    virtual ~SceneInstruction();

    virtual void start();
    virtual void gameLoop(SDL_Event &event);
};