#pragma once
#include "Scene.h"

using namespace std;

class ScenePause : public Scene {
private:
public:
    ScenePause();
    virtual ~ScenePause();

    virtual void start();
    virtual void gameLoop(SDL_Event &event);
};