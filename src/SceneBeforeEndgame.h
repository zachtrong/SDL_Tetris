#pragma once
#include "Scene.h"

using namespace std;

class SceneBeforeEndgame : private Scene {
private:
	static const SDL_Rect RECT_BUTTON_OK;
    shared_ptr<DisplayObject> buttonOk;
public:
    SceneBeforeEndgame();
    virtual ~SceneBeforeEndgame();

    virtual void start();
    virtual SceneType gameLoop(SDL_Event &event);

	void onClickButtonOk();
};