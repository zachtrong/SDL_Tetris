#include "SceneBeforeEndgame.h"

using namespace std;


const SDL_Rect SceneBeforeEndgame::RECT_EDIT_NAME = {
	451, 181,
	366, 70
};

const SDL_Rect SceneBeforeEndgame::RECT_BUTTON_OK = {
	368, 460,
	165, 70
};

SceneBeforeEndgame::SceneBeforeEndgame()
	:buttonOk(new DisplayObject(
        "assets/textures/button_ok_endgame.png", 
        RECT_BUTTON_OK
    )),
    name(new FontObject(
        "", 36, RECT_EDIT_NAME
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
	setButtonDefault(nullptr, nullptr);
	function<void ()> buttonOkCallback = bind(
		&SceneBeforeEndgame::onClickButtonOk,
		this
	);
	addButton(buttonOk, buttonOkCallback);

    SDL_StartTextInput();
}

void SceneBeforeEndgame::redraw() {
    view->renderDisplayObject(background);
    if (name->text.empty()) name->text = "_";
    view->renderFontObject(name, true);
    if (name->text[0] == '_') name->text = "";
}

SceneType SceneBeforeEndgame::gameLoop(SDL_Event &event) {
	Scene::gameLoop(event);
    if (event.type == SDL_TEXTINPUT) {
        string inputText = string(event.text.text);
        if (!inputText.empty() && name->text.size() + inputText.size() <= 16) {
            name->text = name->text + inputText;
            redraw();
        }
    } else if (event.type == SDL_KEYDOWN) {
        if (event.key.keysym.sym == SDLK_BACKSPACE) {
            if (!name->text.empty()) {
                name->text.pop_back();
                redraw();
            }
        }
    }
	return nextSceneType;
}

void SceneBeforeEndgame::onClickButtonOk() {
    SDL_StopTextInput();
	nextSceneType = ENDGAME;
}