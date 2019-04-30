#include "SceneInstruction.h"

using namespace std;

SceneInstruction::SceneInstruction() {
	sceneType = INSTRUCTION;
    background = make_shared<DisplayObject>("assets/textures/instruction.png", RECT_BACKGROUND);
}

SceneInstruction::~SceneInstruction() {

}

void SceneInstruction::start() {
    view->renderDisplayObject(background);
}

void SceneInstruction::gameLoop(SDL_Event &event) {
}