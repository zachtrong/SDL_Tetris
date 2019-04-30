#include "SceneEndgame.h"

using namespace std;

const SDL_Rect SceneEndgame::RECT_BUTTON_OK = {
	368, 460,
	165, 70
};

SceneEndgame::SceneEndgame()
	:buttonOk(new DisplayObject(
        "assets/textures/button_ok_endgame.png", 
        RECT_BUTTON_OK
    ))
{
    background = make_shared<DisplayObject>("assets/textures/scene_endgame.png", RECT_BACKGROUND);
}

SceneEndgame::~SceneEndgame() {

}

void SceneEndgame::start() {
	sceneType = nextSceneType = BEFORE_ENDGAME;
    view->renderDisplayObject(background);

	clearButton();
	setButtonDefault(background, nullptr);
	function<void ()> buttonOkCallback = bind(
		&SceneEndgame::onClickButtonOk,
		this
	);
	addButton(buttonOk, buttonOkCallback);
}

SceneType SceneEndgame::gameLoop(SDL_Event &event) {
	Scene::gameLoop(event);
	return nextSceneType;
}

void SceneEndgame::onClickButtonOk() {
	nextSceneType = START;
}