#include "SceneStart.h"

using namespace std;

const SDL_Rect SceneStart::RECT_BUTTON_INSTRUCTION = {
	393, 273,
	143, 70
};
const SDL_Rect SceneStart::RECT_BUTTON_INSTRUCTION = {
	404, 368,
	120, 33
};

SceneStart::SceneStart() {
	sceneType = START;
    background = make_shared<DisplayObject>("assets/textures/scene_start.png", RECT_BACKGROUND);
    buttonStart = make_shared<DisplayObject>(
        "assets/textures/button_start_mouse_over.png", 
        RECT_BUTTON_START
    );
    buttonInstruction = make_shared<DisplayObject>(
        "assets/textures/button_instruction_mouse_over.png", 
        RECT_BUTTON_INSTRUCTION
    );
}

SceneStart::~SceneStart() {

}

void SceneStart::start() {
    view->renderDisplayObject(background);
}

void SceneStart::gameLoop(SDL_Event &event) {
	if (event.type == SDL_MOUSEMOTION) {
		handleMouseOver();
	} else if (event.type == SDL_MOUSEBUTTONUP) {
		handleMouseClick();
	}
}

void SceneStart::handleMouseOver() {
	if (isMouseOverStartButton()) {
		handleMouseOverStart();
	} else if (isMouseOverInstructionButton()) {
		handleMouseOverInstruction();
	} else {
		handleMouseOverBackground();
	}
}

bool SceneStart::isMouseOverStartButton() {
	return isMouseOverRect(RECT_BUTTON_START);
}

bool SceneStart::isMouseOverRect(const SDL_Rect &rect) {
	int mouseX, mouseY;
	SDL_GetMouseState(&mouseX, &mouseY);
	return (
		rect.x <= mouseX
		&& mouseX <= rect.x + rect.w
		&& rect.y <= mouseY
		&& mouseY <= rect.y + rect.h
	);
}

void SceneStart::handleMouseOverStart() {
	if (!mouseOverStart) {
		mouseOverStart = true;
		onMouseOverButtonStart();
	}
	if (mouseOverInstruction) {
		mouseOverInstruction = false;
		onMouseOutButtonInstruction();
	}
}

bool SceneStart::isMouseOverInstructionButton() {
	return isMouseOverRect(RECT_BUTTON_INSTRUCTION);
}

void SceneStart::handleMouseOverInstruction() {
	if (!mouseOverInstruction) {
		mouseOverInstruction = true; 
        onMouseOverButtonInstruction();
	}
	if (mouseOverStart) {
		mouseOverStart = false;
		onMouseOutButtonStart();
	}
}

void SceneStart::handleMouseClick() {
	if (isMouseOverStartButton()) {
        //TODO
	} else if (isMouseOverInstructionButton()) {
        //TODO
	}
}

void SceneStart::handleMouseOverBackground() {
	if (mouseOverStart) {
		mouseOverStart = false;
		onMouseOutButtonStart();
	}
	if (mouseOverInstruction) {
		mouseOverInstruction = false;
		onMouseOutButtonInstruction();
	}
}

void SceneStart::onMouseOverButtonStart() {
    view->renderDisplayObject(buttonStart);
}

void SceneStart::onMouseOutButtonStart() {
    start();
}

void SceneStart::onMouseOverButtonInstruction() {
    view->renderDisplayObject(buttonInstruction);
}

void SceneStart::onMouseOutButtonInstruction() {
    start();
}