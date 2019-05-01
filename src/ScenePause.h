#pragma once
#include "Scene.h"

using namespace std;

class ScenePause : public Scene {
private:
    static const SDL_Rect RECT_BUTTON_RESUME;
    static const SDL_Rect RECT_BUTTON_RESTART;
    shared_ptr<DisplayObject> buttonResume, buttonRestart;
public:
    ScenePause();
    virtual ~ScenePause();

    virtual void start();
    virtual SceneType gameLoop(SDL_Event &event);

    void onClickButtonResume();
    void onClickButtonRestart();
};