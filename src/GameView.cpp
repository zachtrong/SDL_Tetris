#include "GameView.h"

shared_ptr<GameView> GameView::instance(nullptr);

shared_ptr<GameView> GameView::getInstance() {
    if (instance == nullptr) {
        instance = make_shared<GameView>();
    }
    return instance;
}

GameView::GameView(): 
	window(nullptr),
	renderer(nullptr),
	texture(nullptr) {

}

GameView::~GameView() {
	IMG_Quit();
	SDL_Quit();
}

void GameView::updateBoard(const Board &board) {
    for (int i = 0; i < Constants::BOARD_WIDTH; ++i) {
        for (int j = 0; j < Constants::BOARD_HEIGHT; ++j) {

        }
    }
}

void GameView::startSDL() {
	try {
		init();
	} catch (const Exception &e) {
		printf("Something went wrong!!! %s", e.what());
		return;
	} catch (const char* message) {
		printf("What a Terrible Failure, %s", message);
		return;
	}

	printf("initialization successful!\n");
	
	drawBackground();
	drawRedFilledQuad();
}

void GameView::init() {
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		throw new Exception(SDL_GetError());
		return;
	}
	initWindow();
	initRenderer();
	initImage();
	initTexture();
}

void GameView::initWindow() {
	window = PointerDefinition::createSdlWindow(
		SDL_CreateWindow(
			Constants::GAME_TITLE, 
			SDL_WINDOWPOS_UNDEFINED, 
			SDL_WINDOWPOS_UNDEFINED, 
			Constants::SCREEN_WIDTH, 
			Constants::SCREEN_HEIGHT, 
			SDL_WINDOW_SHOWN
		)
	);
	if (window == nullptr) {
		throw new Exception(SDL_GetError());
	}
}

void GameView::initRenderer() {
	renderer = PointerDefinition::createSdlRenderer(
		SDL_CreateRenderer(window.get(), -1, SDL_RENDERER_ACCELERATED)
	);
	if (renderer == nullptr) {
		throw new Exception(SDL_GetError());
	}
}

void GameView::initImage() {
	int imageFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imageFlags) && imageFlags)) {
		throw new Exception(IMG_GetError());
	}
}

void GameView::initTexture() {
	texture = PointerDefinition::createSdlTexture(
		loadBackgroundTexture()
	);
	if (texture == nullptr) {
		throw new Exception(SDL_GetError());
	}
}

SDL_Texture* GameView::loadBackgroundTexture() {
	shared_ptr<SDL_Surface> loadedSurface(IMG_Load("assets/textures/background.png"));
	if (loadedSurface == nullptr) {
		throw new Exception(IMG_GetError());
	}
	return SDL_CreateTextureFromSurface(renderer.get(), loadedSurface.get());
}

void GameView::drawBackground() {
	SDL_SetRenderDrawColor(renderer.get(), 0xff, 0xff, 0xff, 0xff);

	SDL_RenderClear(renderer.get());
	SDL_RenderCopy(renderer.get(), texture.get(), NULL, NULL);
	SDL_RenderPresent(renderer.get());
}

void GameView::drawRedFilledQuad() {
	//Render red filled quad
	SDL_Rect fillRect = { 
		Constants::SCREEN_WIDTH / 4, 
		Constants::SCREEN_HEIGHT / 4, 
		Constants::SCREEN_WIDTH / 2, 
		Constants::SCREEN_HEIGHT / 2 
	};
	SDL_SetRenderDrawColor( renderer.get(), 0xFF, 0x00, 0x00, 0xFF );
	SDL_RenderFillRect( renderer.get(), &fillRect );
	SDL_RenderPresent(renderer.get());
}
