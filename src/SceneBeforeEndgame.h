#pragma once
#include "Scene.h"

using namespace std;

class SceneBeforeEndgame : private Scene {
private:
	static const SDL_Rect RECT_EDIT_NAME;
	static const SDL_Rect RECT_BUTTON_OK;
    shared_ptr<DisplayObject> buttonOk;

    static shared_ptr<FontObject> player;
public:
    SceneBeforeEndgame();
    virtual ~SceneBeforeEndgame();

    virtual void start();
    virtual void redraw();
    virtual SceneType gameLoop(SDL_Event &event);

    static string getPlayerName();

	void onClickButtonOk();
};