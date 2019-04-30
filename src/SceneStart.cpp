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

SceneStart::SceneStart()
	:buttonStart(new DisplayObject(
        "assets/textures/button_start_mouse_over.png", 
        RECT_BUTTON_START
    )),
    buttonInstruction(new DisplayObject(
        "assets/textures/button_instruction_mouse_over.png", 
        RECT_BUTTON_INSTRUCTION
    ))
{
	sceneType = START;
    background = make_shared<DisplayObject>("assets/textures/scene_start.png", RECT_BACKGROUND);
}

SceneStart::~SceneStart() {

}

void SceneStart::start() {
    view->renderDisplayObject(background);

	clearButton();
	function<void ()> buttonStartCallback = bind(
		&SceneStart::onMouseClickButtonStart,
		this
	);
	addButton(buttonStart, buttonStartCallback);
	function<void ()> buttonInstructionCallback = bind(
		&SceneStart::onMouseClickButtonInstruction,
		this
	);
	addButton(buttonInstruction, buttonInstructionCallback);
}

void SceneStart::gameLoop(SDL_Event &event) {
	Scene::gameLoop(event);
}

void SceneStart::onMouseClickButtonStart() {

}

void SceneStart::onMouseClickButtonInstruction() {

}