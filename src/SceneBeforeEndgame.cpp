#include "SceneBeforeEndgame.h"

using namespace std;


const SDL_Rect SceneBeforeEndgame::RECT_EDIT_NAME = {
	451, 185,
	366, 70
};

const SDL_Rect SceneBeforeEndgame::RECT_BUTTON_OK = {
	368, 460,
	165, 70
};

shared_ptr<FontObject> SceneBeforeEndgame::player(new FontObject(
    " ", 36, RECT_EDIT_NAME
));

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
    view->renderFontObject(player, true);
}

SceneType SceneBeforeEndgame::gameLoop(SDL_Event &event) {
	Scene::gameLoop(event);
    if (event.type == SDL_TEXTINPUT) {
        string inputText = string(event.text.text);
        if (!inputText.empty()) {
            if (player->text[0] == ' ') {
                player->text = inputText;
            } else if (player->text.size() + inputText.size() <= 16) {
                player->text = player->text + inputText;
            }
            redraw();
        }
    } else if (event.type == SDL_KEYDOWN) {
        if (event.key.keysym.sym == SDLK_BACKSPACE) {
            if (!player->text.empty()) {
                player->text.pop_back();
                if (player->text.empty()) {
                    player->text = " ";
                }
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

string SceneBeforeEndgame::getPlayerName() {
    return player->text;
}