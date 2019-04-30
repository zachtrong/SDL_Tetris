#include "Scene.h"
#include "SceneStart.h"
#include "SceneInstruction.h"
#include "ScenePlay.h"
#include "ScenePause.h"
#include "SceneBeforeEndgame.h"
#include "SceneEndgame.h"

shared_ptr<GameView> Scene::view = GameView::getInstance();
shared_ptr<GameController> Scene::controller = GameController::getInstance();
shared_ptr<GameSound> Scene::sound = GameSound::getInstance();

const SDL_Rect Scene::RECT_BACKGROUND = {
	0, 0, 
	Constants::SCREEN_WIDTH, Constants::SCREEN_HEIGHT
};

Scene::Scene()
	:background(),
	displayObjects(),
	sceneType(),
	nextSceneType()
{
}

Scene::~Scene() {

}

shared_ptr<Scene> Scene::createSceneFromSceneType(SceneType sceneType) {
	switch (sceneType) {
		case START: 
			return shared_ptr<Scene>((Scene*) new SceneStart());
		case PLAY: 
			return shared_ptr<Scene>((Scene*) new ScenePlay());
		case PAUSE: 
			return shared_ptr<Scene>((Scene*) new ScenePause());
		case INSTRUCTION: 
			return shared_ptr<Scene>((Scene*) new SceneInstruction());
		case BEFORE_ENDGAME:
			return shared_ptr<Scene>((Scene*) new SceneBeforeEndgame());
		case ENDGAME:
			return shared_ptr<Scene>((Scene*) new SceneEndgame());
		default:
			throw "NO SUCH SCENE!";
	}
}

void Scene::start() {
}

SceneType Scene::gameLoop(SDL_Event &event) {
	if (event.type == SDL_MOUSEMOTION) {
		handleMouse([this](Button button){
			if (button.displayObject == nullptr) {
				redraw();
			} else {
				view->renderDisplayObject(button.displayObject);
			}
		}, [](__attribute__((unused)) Button button){});
	} else if (event.type == SDL_MOUSEBUTTONUP) {
		handleMouse([](__attribute__((unused)) Button button){}, 
		[](Button button){
			if (button.callback != nullptr) {
				button.callback();
			}
		});
	}
	return sceneType;
}

void Scene::handleMouse(function<void (Button button)> callbackRender, function<void (Button button)> callbackClick) {
	bool anyInside = false;
	for (size_t i = 0; i < buttons.size(); ++i) {
		auto button = buttons[i];
		if (isMouseOverRect(button.displayObject->rect)) {
			anyInside = true;
			togglingButtonDefaultState = false;
			callbackClick(button);
			if (!togglingButtonStates[i]) {
				togglingButtonStates[i] = true;
				callbackRender(button);
			}
		} else {
			togglingButtonStates[i] = false;
		}
	}
	if (!anyInside) {
		callbackClick(buttonDefault);
		if (!togglingButtonDefaultState) {
			togglingButtonDefaultState = true;
			callbackRender(buttonDefault);
		}
	}
}

bool Scene::isMouseOverRect(const SDL_Rect &rect) {
	int mouseX, mouseY;
	SDL_GetMouseState(&mouseX, &mouseY);
	return (
		rect.x <= mouseX
		&& mouseX <= rect.x + rect.w
		&& rect.y <= mouseY
		&& mouseY <= rect.y + rect.h
	);
}

void Scene::addButton(shared_ptr<DisplayObject> buttonView, function<void ()> onMouseClick) {
	buttons.push_back(Button(buttonView, onMouseClick));
    togglingButtonStates.push_back(false);
}

void Scene::setButtonDefault(shared_ptr<DisplayObject> buttonView, function<void ()> onMouseClick) {
	buttonDefault = Button(buttonView, onMouseClick);
	togglingButtonDefaultState = false;
}

void Scene::redraw() {

}

void Scene::clearButton() {
	buttons.clear();
	togglingButtonStates.clear();
	buttonDefault = Button();
	togglingButtonDefaultState = false;
}

SceneType Scene::getType() {
	return sceneType;
}