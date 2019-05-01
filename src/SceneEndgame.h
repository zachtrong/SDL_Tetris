#pragma once
#include "Scene.h"
#include "GameController.h"

using namespace std;

class SceneEndgame : private Scene {
private:
	static const SDL_Rect RECT_BUTTON_OK;
    static const SDL_Rect RECT_RANKING_TOP_NAME[3];
    static const SDL_Rect RECT_RANKING_USER_NAME;
    static const SDL_Rect RECT_RANKING_TOP_SCORE[3];
    static const SDL_Rect RECT_RANKING_USER_SCORE;

    static shared_ptr<GameController> controller;
    shared_ptr<DisplayObject> buttonOk;

    vector<shared_ptr<Score>> rankings;
    shared_ptr<Score> player;
public:
    SceneEndgame();
    virtual ~SceneEndgame();

    virtual void start();
    virtual void redraw();
    virtual SceneType gameLoop(SDL_Event &event);

    void onClickButtonOk();
};