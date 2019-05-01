#include "ScenePause.h"

using namespace std;

const SDL_Rect ScenePause::RECT_BUTTON_RESUME = {
    222, 446,
    187, 70
};

const SDL_Rect ScenePause::RECT_BUTTON_RESTART = {
    494, 446,
    187, 70
};

ScenePause::ScenePause() 
    :buttonResume(new DisplayObject(
        "assets/textures/button_resume.png",
        RECT_BUTTON_RESUME
    )),
    buttonRestart(new DisplayObject(
        "assets/textures/button_restart.png",
        RECT_BUTTON_RESTART
    ))
{
    background = make_shared<DisplayObject>("assets/textures/scene_pause.png", RECT_BACKGROUND);
}

ScenePause::~ScenePause() {
}

void ScenePause::start() {
	sceneType = nextSceneType = PAUSE;
    view->renderDisplayObject(background);

	clearButton();
    setButtonDefault(background, nullptr);
	function<void ()> buttonResumeCallback = bind(
		&ScenePause::onClickButtonResume,
		this
	);
    addButton(buttonResume, buttonResumeCallback);
	function<void ()> buttonRestartCallback = bind(
		&ScenePause::onClickButtonRestart,
		this
	);
    addButton(buttonRestart, buttonRestartCallback);
}

SceneType ScenePause::gameLoop(SDL_Event &event) {
    Scene::gameLoop(event);
    return nextSceneType;
}

void ScenePause::onClickButtonResume() {
    nextSceneType = BACK_TO_PREVIOUS;
}

void ScenePause::onClickButtonRestart() {
    nextSceneType = PLAY;
}