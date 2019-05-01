#pragma once
#include "Scene.h"

using namespace std;

class SceneStart : private Scene {
private:
	static const SDL_Rect RECT_BUTTON_START;
	static const SDL_Rect RECT_BUTTON_INSTRUCTION;
	static const SDL_Rect RECT_BUTTON_HIGH_SCORE;

    shared_ptr<DisplayObject> buttonStart, buttonInstruction, buttonHighScore;
public:
    SceneStart();
    virtual ~SceneStart();

    virtual void start();
    virtual SceneType gameLoop(SDL_Event &event);

	void onClickButtonStart();
	void onClickButtonInstruction();
	void onClickButtonHighScore();
};