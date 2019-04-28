#pragma once
#include "Scene.h"

using namespace std;

class SceneStart : private Scene {
private:
	static const SDL_Rect RECT_BUTTON_START;
	static const SDL_Rect RECT_BUTTON_INSTRUCTION;

    shared_ptr<DisplayObject> buttonStart, buttonInstruction;

	bool mouseOverStart;
	bool mouseOverInstruction;

	void handleMouseClick();
	void handleMouseOver();
	void handleMouseOverStart();
	void handleMouseOverInstruction();
	void handleMouseOverBackground();
	bool isMouseOverStartButton();
	bool isMouseOverInstructionButton();
	bool isMouseOverRect(const SDL_Rect &rect);
	void onMouseOverButtonStart();
	void onMouseOutButtonStart();
	void onMouseOverButtonInstruction();
	void onMouseOutButtonInstruction();
public:
    SceneStart();
    virtual ~SceneStart();

    virtual void start();
    virtual void gameLoop(SDL_Event &event);
};