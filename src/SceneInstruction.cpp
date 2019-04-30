#include "SceneInstruction.h"

using namespace std;

const SDL_Rect SceneInstruction::RECT_BUTTON_OK = {
    371, 445,
    165, 70
};

SceneInstruction::SceneInstruction() 
    :buttonOk(new DisplayObject(
        "assets/textures/button_ok.png",
        RECT_BUTTON_OK
    ))
{
	sceneType = INSTRUCTION;
    background = make_shared<DisplayObject>("assets/textures/scene_instruction.png", RECT_BACKGROUND);
}

SceneInstruction::~SceneInstruction() {

}

void SceneInstruction::start() {
    view->renderDisplayObject(background);

	clearButton();
	function<void ()> buttonOkCallback = bind(
		&SceneInstruction::onClickButtonOk,
		this
	);
	addButton(buttonOk, buttonOkCallback);
}

void SceneInstruction::gameLoop(SDL_Event &event) {
    Scene::gameLoop(event);
}

void SceneInstruction::onClickButtonOk() {
    //TODO
}