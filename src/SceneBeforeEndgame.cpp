#include "SceneBeforeEndgame.h"

using namespace std;

const SDL_Rect SceneBeforeEndgame::RECT_BUTTON_OK = {
	368, 460,
	165, 70
};

SceneBeforeEndgame::SceneBeforeEndgame()
	:buttonOk(new DisplayObject(
        "assets/textures/button_ok_endgame.png", 
        RECT_BUTTON_OK
    ))
{
    background = make_shared<DisplayObject>("assets/textures/scene_before_endgame.png", RECT_BACKGROUND);
}

SceneBeforeEndgame::~SceneBeforeEndgame() {

}

void SceneBeforeEndgame::start() {
	sceneType = nextSceneType = BEFORE_ENDGAME;
    view->renderDisplayObject(background);

	clearButton();
	setButtonDefault(background, nullptr);
	function<void ()> buttonOkCallback = bind(
		&SceneBeforeEndgame::onClickButtonOk,
		this
	);
	addButton(buttonOk, buttonOkCallback);
}

SceneType SceneBeforeEndgame::gameLoop(SDL_Event &event) {
	Scene::gameLoop(event);
	return nextSceneType;
}

void SceneBeforeEndgame::onClickButtonOk() {
	nextSceneType = ENDGAME;
}