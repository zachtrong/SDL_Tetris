#include "ScenePause.h"

using namespace std;

ScenePause::ScenePause() {
	sceneType = PAUSE;
    background = make_shared<DisplayObject>("assets/textures/scene_pause.png", RECT_BACKGROUND);
}

ScenePause::~ScenePause() {

}

void ScenePause::start() {
    view->renderDisplayObject(background);
}

void ScenePause::gameLoop(SDL_Event &event) {
}