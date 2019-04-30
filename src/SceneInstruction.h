#pragma once
#include "Scene.h"

using namespace std;

class SceneInstruction : public Scene {
private:
    static const SDL_Rect RECT_BUTTON_OK;
    shared_ptr<DisplayObject> buttonOk;
public:
    SceneInstruction();
    virtual ~SceneInstruction();

    virtual void start();
    virtual SceneType gameLoop(SDL_Event &event);

    void onClickButtonOk();
};