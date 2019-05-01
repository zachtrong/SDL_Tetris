#pragma once
#include "Scene.h"

using namespace std;

struct Score {
    shared_ptr<FontObject> user, score;

    Score(): user(), score() {}
    Score(shared_ptr<FontObject> _user, shared_ptr<FontObject> _score): user(_user), score(_score) {}
};

class SceneEndgame : private Scene {
private:
	static const SDL_Rect RECT_BUTTON_OK;
    static const SDL_Rect RECT_RANKING_TOP_NAME[3];
    static const SDL_Rect RECT_RANKING_USER_NAME;
    static const SDL_Rect RECT_RANKING_TOP_SCORE[3];
    static const SDL_Rect RECT_RANKING_USER_SCORE;
    shared_ptr<DisplayObject> buttonOk;

    vector<Score> rankings;
    vector<Score> player;
public:
    SceneEndgame();
    virtual ~SceneEndgame();

    virtual void start();
    virtual void redraw();
    virtual SceneType gameLoop(SDL_Event &event);

    void onClickButtonOk();
};