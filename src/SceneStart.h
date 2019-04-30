#pragma once
#include "Scene.h"

using namespace std;

class SceneStart : private Scene {
private:
	static const SDL_Rect RECT_BUTTON_START;
	static const SDL_Rect RECT_BUTTON_INSTRUCTION;

    shared_ptr<DisplayObject> buttonStart, buttonInstruction;
public:
    SceneStart();
    virtual ~SceneStart();

    virtual void start();
    virtual void gameLoop(SDL_Event &event);

	void onMouseClickButtonStart();
	void onMouseClickButtonInstruction();
};