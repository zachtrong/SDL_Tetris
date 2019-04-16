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


	Board b;

	b[0][0].setType(O);
	b[0][1].setType(L);
	b[0][2].setType(J);
	b[1][0].setType(S);
	b[1][1].setType(L);
	b[1][2].setType(T);
	b[2][0].setType(O);
	b[2][1].setType(J);

	updateBoard(b);
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
	windowSurface = PointerDefinition::createSdlSurface(
		SDL_GetWindowSurface(window.get())
	);
}

void GameView::initRenderer() {
	renderer = PointerDefinition::createSdlRenderer(
		SDL_CreateRenderer(window.get(), -1, SDL_RENDERER_ACCELERATED)
	);
	if (renderer == nullptr) {
		throw new Exception(SDL_GetError());
	}
	SDL_SetRenderDrawBlendMode(renderer.get(), SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer.get(), 0, 0, 0, 0);
}

void GameView::initImage() {
	int imageFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imageFlags) && imageFlags)) {
		throw new Exception(IMG_GetError());
	}
}

void GameView::initTexture() {
	texture = createTexture(Constants::BACKGROUND_IMG_PATH);
	if (texture == nullptr) {
		throw new Exception(SDL_GetError());
	}
}

shared_ptr<SDL_Texture> GameView::createTexture(string path) {
	shared_ptr<SDL_Surface> loadedSurface = createSurface(path);
	if (loadedSurface == nullptr) {
		throw new Exception(IMG_GetError());
	}
	SDL_SetColorKey(loadedSurface.get(), SDL_TRUE, SDL_MapRGBA(loadedSurface->format, 0xff, 0xff, 0xff, 0x00));
	shared_ptr<SDL_Texture> res = PointerDefinition::createSdlTexture(
		SDL_CreateTextureFromSurface(renderer.get(), loadedSurface.get())
	);
	SDL_SetTextureBlendMode(res.get(), SDL_BLENDMODE_BLEND);
	return res;
}

shared_ptr<SDL_Surface> GameView::createSurface(string path) {
	shared_ptr<SDL_Surface> loadedRawSurface(IMG_Load(path.c_str()));
	shared_ptr<SDL_Surface> loadedSurface(
		SDL_ConvertSurface(loadedRawSurface.get(), windowSurface->format, 0)
	);
	return loadedSurface;
}

void GameView::drawBackground() {
	SDL_RenderClear(renderer.get());
	SDL_RenderCopy(renderer.get(), texture.get(), nullptr, nullptr);
	SDL_RenderPresent(renderer.get());
}

void GameView::updateBoard(Board &board) {
    for (int i = 0; i < Constants::BOARD_HEIGHT; ++i) {
        for (int j = 0; j < Constants::BOARD_WIDTH; ++j) {
			copyTileToRenderer(&board[i][j]);
        }
    }
	SDL_RenderPresent(renderer.get());
}

void GameView::copyTileToRenderer(Tile *t) {
	auto tileTexture = createTexture(t->getAssetPath());
	SDL_Rect &rect = t->getPositionOnWindow();
	SDL_RenderCopy(renderer.get(), tileTexture.get(), nullptr, &rect);
}

void GameView::updateBoardChangedPositions(Board &board, vector<SDL_Point> &positions) {
	for (SDL_Point p : positions) {
		copyTileToRenderer(&board[p.x][p.y]);
	}
	SDL_RenderPresent(renderer.get());
}