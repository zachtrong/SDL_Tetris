#include "SceneEndgame.h"
#include "SceneBeforeEndgame.h"

using namespace std;

const string SceneEndgame::DATA_RANKING_FILE = "data_ranking.txt";
const SDL_Rect SceneEndgame::RECT_BUTTON_OK = {
	368, 460,
	165, 70
};
const SDL_Rect SceneEndgame::RECT_RANKING_TOP_NAME[3] = {
	{180, 185, 275, 54},
	{180, 239, 275, 54},
	{180, 293, 275, 54}
};
const SDL_Rect SceneEndgame::RECT_RANKING_USER_NAME = {
	181, 371,
	275, 54
};
const SDL_Rect SceneEndgame::RECT_RANKING_TOP_SCORE[3] = {
	{717, 185, 275, 54},
	{717, 239, 275, 54},
	{717, 293, 275, 54},
};
const SDL_Rect SceneEndgame::RECT_RANKING_USER_SCORE = {
	717, 371,
	275, 54
};

shared_ptr<GameController> SceneEndgame::controller = GameController::getInstance();

SceneEndgame::SceneEndgame()
	:buttonOk(new DisplayObject(
        "assets/textures/button_ok_endgame.png", 
        RECT_BUTTON_OK
    )),
	rankings(),
	player(make_shared<Score>(
		make_shared<FontObject>(" ", 36, RECT_RANKING_USER_NAME, true),
		make_shared<FontObject>("0", 36, RECT_RANKING_USER_SCORE, true)
	))
{
    background = make_shared<DisplayObject>("assets/textures/scene_endgame.png", RECT_BACKGROUND);
}

void SceneEndgame::setUpData() {
	player->user->text = SceneBeforeEndgame::getPlayerName();
	player->score->text = to_string(controller->getScore());

	fstream fs;
	fs.open("data_ranking.txt", fstream::in);
	vector<pair<string, string>> data(0);
	bool isDataHavingCurrentPlayer = false;
	for (size_t i = 0; i < 3u; ++i) {
		string user = " ";
		string score = "0";
		getline(fs, user);
		getline(fs, score);
		if (user == player->user->text && score == player->score->text) {
			isDataHavingCurrentPlayer = true;
		}
		data.emplace_back(user, score);
		if (!isDataHavingCurrentPlayer) {
			auto dataTop = data.back();
			if (stoi(data.back().second) < controller->getScore()) {
				isDataHavingCurrentPlayer = true;
				data.pop_back();
				data.emplace_back(player->user->text, player->score->text);
				data.push_back(dataTop);
			}
		}
	}
	while (data.size() > 3u) {
		data.pop_back();
	}
	fs.close();

	fs.open("data_ranking.txt", fstream::out);
	for (size_t i = 0; i < data.size(); ++i) {
		fs << data[i].first << endl;
		fs << data[i].second << endl;
		rankings.push_back(make_shared<Score>(make_shared<FontObject>(
			data[i].first, 36,
			RECT_RANKING_TOP_NAME[i], true
		), make_shared<FontObject>(
			data[i].second, 36,
			RECT_RANKING_TOP_SCORE[i], true
		)));
	}
	fs.close();
}

SceneEndgame::~SceneEndgame() {

}

void SceneEndgame::start() {
	setUpData();
	sceneType = nextSceneType = BEFORE_ENDGAME;
	redraw();

	clearButton();
	function<void ()> buttonOkCallback = bind(
		&SceneEndgame::onClickButtonOk,
		this
	);
	addButton(buttonOk, buttonOkCallback);
}

void SceneEndgame::redraw() {
	view->renderDisplayObject(background);
	for (auto score : rankings) {
		view->renderFontObject(score->user);
		view->renderFontObject(score->score, 2);
	}
	view->renderFontObject(player->user);
	view->renderFontObject(player->score, 2);
}

SceneType SceneEndgame::gameLoop(SDL_Event &event) {
	Scene::gameLoop(event);
	return nextSceneType;
}

void SceneEndgame::onClickButtonOk() {
	nextSceneType = START;
}