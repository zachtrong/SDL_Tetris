#pragma once
#include "Scene.h"

using namespace std;

class SceneEndgame : private Scene {
private:
	static const SDL_Rect RECT_BUTTON_OK;
    shared_ptr<DisplayObject> buttonOk;
public:
    SceneEndgame();
    virtual ~SceneEndgame();

    virtual void start();
    virtual SceneType gameLoop(SDL_Event &event);

    void onClickButtonOk();
};