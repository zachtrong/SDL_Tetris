#include "GameView.h"

shared_ptr<GameView> GameView::instance(nullptr);

shared_ptr<GameView> GameView::getInstance() {
    if (instance == nullptr) {
        instance = make_shared<GameView>();
    }
    return instance;
}

const SDL_Rect GameView::RECT_BACKGROUND = {
	0, 0, 
	Constants::SCREEN_WIDTH, Constants::SCREEN_HEIGHT
};
const SDL_Rect GameView::RECT_BORDER_LEFT = {
	Constants::SCREEN_OFFSET - Constants::BOARD_BORDER_SIZE, 0,
	Constants::BOARD_BORDER_SIZE, Constants::SCREEN_HEIGHT
};
const SDL_Rect GameView::RECT_BORDER_RIGHT = {
	Constants::SCREEN_OFFSET*2, 0,
	Constants::BOARD_BORDER_SIZE, Constants::SCREEN_HEIGHT
};
const SDL_Rect GameView::RECT_BACKGROUND_SCORE = {
	0, 353, 
	Constants::SCREEN_OFFSET - Constants::BOARD_BORDER_SIZE, 54
};
const SDL_Rect GameView::RECT_HOLDING = { 89, 100, 122, 105, };
const SDL_Rect GameView::RECT_PREPARING[4] = {
	{Constants::SCREEN_OFFSET*2 + 89, 100, 122, 105},
	{Constants::SCREEN_OFFSET*2 + 98, 234, 105, 90},
	{Constants::SCREEN_OFFSET*2 + 105, 353, 91, 78},
	{Constants::SCREEN_OFFSET*2 + 105, 460, 91, 78}
};

GameView::GameView(): 
	window(nullptr),
	windowSurface(nullptr),
	renderer(nullptr),
	tileTextures(),
	fullTileTextures(),
	scoringFont(nullptr),
	colorWhite({0xff, 0xff, 0xff, 0xff}),
	colorBackground({0x55, 0x60, 0x60, 0xff})
{

}

GameView::~GameView() {
	IMG_Quit();
	SDL_Quit();
	TTF_Quit();
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
}

void GameView::init() {
	initWindow();
	initRenderer();
	initImage();
	initTileTexture();
	initTextureText();
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
		throw Exception(SDL_GetError());
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
		throw Exception(SDL_GetError());
	}
	SDL_SetRenderDrawBlendMode(renderer.get(), SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(
		renderer.get(), 
		colorBackground.r, 
		colorBackground.g, 
		colorBackground.b, 
		colorBackground.a
	);
}

void GameView::initImage() {
	int imageFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imageFlags) && imageFlags)) {
		throw Exception(IMG_GetError());
	}
}

shared_ptr<SDL_Texture> GameView::createTexture(const string &path) {
	shared_ptr<SDL_Surface> loadedSurface = createSurface(path);
	if (loadedSurface == nullptr) {
		throw Exception(IMG_GetError());
	}
	auto res = PointerDefinition::createSdlTexture(
		SDL_CreateTextureFromSurface(renderer.get(), loadedSurface.get())
	);
	return res;
}

shared_ptr<SDL_Texture> GameView::createTextureTransparent(const string &path) {
	shared_ptr<SDL_Surface> loadedSurface = createSurface(path);
	if (loadedSurface == nullptr) {
		throw Exception(IMG_GetError());
	}
	SDL_SetColorKey(loadedSurface.get(), SDL_RLEACCEL, loadedSurface->format->Amask);
	shared_ptr<SDL_Texture> res = PointerDefinition::createSdlTexture(
		SDL_CreateTextureFromSurface(renderer.get(), loadedSurface.get())
	);
	SDL_SetTextureBlendMode(res.get(), SDL_BLENDMODE_BLEND);
	return res;
}

shared_ptr<SDL_Surface> GameView::createSurface(const string &path) {
	shared_ptr<SDL_Surface> loadedRawSurface = PointerDefinition::createSdlSurface(
		IMG_Load(path.c_str())
	);
	return loadedRawSurface;
}

void GameView::initTileTexture() {
	tileTextures[EMPTY] = createTextureTransparent(Tile(EMPTY).getAssetPath());
	fullTileTextures[EMPTY] = createTextureTransparent(Tile(EMPTY).getAssetFullTilePath());
	for (const auto &tileType : Constants::MAP_TILE_TYPE) {
		tileTextures[tileType] = createTextureTransparent(Tile(tileType).getAssetPath());
		fullTileTextures[tileType] = createTextureTransparent(Tile(tileType).getAssetFullTilePath());
	}
}

void GameView::initTextureText() {
	if (TTF_Init() == -1) {
		throw Exception(TTF_GetError());
	}

	scoringFont = PointerDefinition::createTtfFont(
		TTF_OpenFont("assets/fonts/agency-fb-bold.ttf", 50)
	);
}

shared_ptr<SDL_Texture> GameView::createTextureText(const string &text, int fontSize, SDL_Rect *rect) {
	auto font = PointerDefinition::createTtfFont(
		TTF_OpenFont("assets/fonts/agency-fb.ttf", fontSize)
	);
	shared_ptr<SDL_Surface> surfaceMessage = PointerDefinition::createSdlSurface(
		TTF_RenderText_Blended(font.get(), text.c_str(), colorWhite)
	);
	*rect = surfaceMessage->clip_rect;
	shared_ptr<SDL_Texture> textureMessage = PointerDefinition::createSdlTexture(
		SDL_CreateTextureFromSurface(renderer.get(), surfaceMessage.get())
	);
	return textureMessage;
}

shared_ptr<SDL_Texture> GameView::createTextureTextScoring(const string &text, SDL_Rect *rect) {
	shared_ptr<SDL_Surface> surfaceMessage = PointerDefinition::createSdlSurface(
		TTF_RenderText_Blended(scoringFont.get(), text.c_str(), colorWhite)
	);
	*rect = surfaceMessage->clip_rect;
	shared_ptr<SDL_Texture> textureMessage = PointerDefinition::createSdlTexture(
		SDL_CreateTextureFromSurface(renderer.get(), surfaceMessage.get())
	);
	return textureMessage;
}

void GameView::drawScenePlay() {
	drawBackground();
	drawTextureText();

	updateScore(0);
	Tile empty(EMPTY);
	vector<Tile> empties({empty, empty, empty, empty});
	updateHoldingTile(empty);
	updatePreparingTile(empties);
}

void GameView::drawBackground() {
	SDL_SetRenderDrawColor(
		renderer.get(), 
		colorBackground.r, colorBackground.g, colorBackground.b, 
		colorBackground.a
	);
	SDL_RenderClear(renderer.get());
	SDL_RenderFillRect(renderer.get(), &RECT_BACKGROUND);
	drawLinesOnBackground();
	SDL_RenderPresent(renderer.get());
}

void GameView::drawLinesOnBackground() {
	SDL_SetRenderDrawColor(renderer.get(), 0x71, 0x71, 0x71, 0xff);
	SDL_RenderFillRect(renderer.get(), &RECT_BORDER_LEFT);
	SDL_RenderFillRect(renderer.get(), &RECT_BORDER_RIGHT);
}

void GameView::drawTextureText() {
	SDL_SetRenderDrawColor(
		renderer.get(), 
		colorWhite.r, colorWhite.g, colorWhite.b, 
		colorWhite.a
	);
	drawTextureHold();
	drawTextureNext();
	drawTextureScore();
	drawTextureFooter();
	SDL_RenderPresent(renderer.get());
}

void GameView::drawTextureHold() {
	SDL_Rect rect;
	auto textureHold = createTextureText("hold", 36, &rect);
	rect.x = 121;
	rect.y = 41;
	SDL_RenderCopy(renderer.get(), textureHold.get(), NULL, &rect);
}

void GameView::drawTextureNext() {
	SDL_Rect rect;
	auto textureNext = createTextureText("next", 36, &rect);
	rect.x = 731;
	rect.y = 41;
	SDL_RenderCopy(renderer.get(), textureNext.get(), NULL, &rect);
}

void GameView::drawTextureScore() {
	SDL_SetRenderDrawColor(
		renderer.get(), 
		colorBackground.r, 
		colorBackground.g, 
		colorBackground.b, 
		colorBackground.a
	);
	SDL_Rect rect;
	auto textureScore = createTextureText("score", 36, &rect);
	rect.x = 112;
	rect.y = 298;
	SDL_RenderCopy(renderer.get(), textureScore.get(), NULL, &rect);
}

void GameView::drawTextureFooter() {
	SDL_Rect rect;
	auto textureFooter = createTextureText("press ESC or P to pause", 24, &rect);
	rect.x = 59;
	rect.y = 555;
	SDL_RenderCopy(renderer.get(), textureFooter.get(), NULL, &rect);
}

void GameView::drawSceneStart() {
	drawSceneStartBackground();
	SDL_SetRenderDrawColor(
		renderer.get(), 0x00, 0x00, 0x00, 0x00
	);
	auto startButton = createTextureTransparent("assets/textures/button_start_default.png");
	SDL_RenderCopy(renderer.get(), startButton.get(), nullptr, &Constants::RECT_START_BUTTON);
	auto instructionButton = createTextureTransparent("assets/textures/button_instruction_default.png");
	SDL_RenderCopy(renderer.get(), instructionButton.get(), nullptr, &Constants::RECT_INSTRUCTION_BUTTON);
	SDL_RenderPresent(renderer.get());
}

void GameView::drawSceneStartBackground() {
	SDL_RenderClear(renderer.get());
	shared_ptr<SDL_Texture> backgroundTexture = createTexture("assets/textures/scene_start.png");
	SDL_RenderCopy(renderer.get(), backgroundTexture.get(), nullptr, &RECT_BACKGROUND);
	SDL_RenderPresent(renderer.get());
}

void GameView::onMouseOverButtonStart() {
	auto startButton = createTextureTransparent("assets/textures/button_start_mouse_over.png");
	SDL_RenderCopy(renderer.get(), startButton.get(), nullptr, &Constants::RECT_START_BUTTON);
	SDL_RenderPresent(renderer.get());
}

void GameView::onMouseOutButtonStart() {
	drawSceneStart();
}

void GameView::onMouseOverButtonInstruction() {
	auto instructionButton = createTextureTransparent("assets/textures/button_instruction_mouse_over.png");
	SDL_RenderCopy(renderer.get(), instructionButton.get(), nullptr, &Constants::RECT_INSTRUCTION_BUTTON);
	SDL_RenderPresent(renderer.get());
}

void GameView::onMouseOutButtonInstruction() {
	drawSceneStart();
}

void GameView::drawScenePause() {
	auto pauseTexture = createTextureTransparent("assets/textures/scene_pause.png");
	SDL_RenderCopy(renderer.get(), pauseTexture.get(), nullptr, &RECT_BACKGROUND);
	SDL_RenderPresent(renderer.get());
}

void GameView::drawSceneInstruction() {
	auto instructionTexture = createTextureTransparent("assets/textures/instruction.png");
	SDL_RenderCopy(renderer.get(), instructionTexture.get(), nullptr, &RECT_BACKGROUND);
	SDL_RenderPresent(renderer.get());
}

void GameView::updateBoard(Board &board) {
    for (int i = Constants::BOARD_HEIGHT/2; i < Constants::BOARD_HEIGHT; ++i) {
        for (int j = 0; j < Constants::BOARD_WIDTH; ++j) {
			copyTileToRenderer(&board[i][j]);
        }
    }
	SDL_RenderPresent(renderer.get());
}

void GameView::copyTileToRenderer(Tile *tile) {
	auto tileTexture = tileTextures[tile->getType()];
	SDL_Rect &rect = tile->getPositionOnWindow();
	SDL_RenderCopy(renderer.get(), tileTexture.get(), nullptr, &rect);
}

void GameView::updateScore(int score) {
	SDL_RenderFillRect(renderer.get(), &RECT_BACKGROUND_SCORE);

	SDL_Rect rect;
	auto textureScorePoint = createTextureTextScoring(to_string(score), &rect);
	rect.x = 144 - rect.w / 2;
	rect.y = 353;

	SDL_RenderCopy(renderer.get(), textureScorePoint.get(), NULL, &rect);
	SDL_RenderPresent(renderer.get());
}

void GameView::updateHoldingTile(Tile &tile) {
	copyFullTileToRenderer(&tile, RECT_HOLDING);
	SDL_RenderPresent(renderer.get());
}

void GameView::copyFullTileToRenderer(Tile *tile, const SDL_Rect &rect) {
	auto fullTileTexture = fullTileTextures[tile->getType()];
	SDL_RenderCopy(renderer.get(), fullTileTexture.get(), nullptr, &rect);
}

void GameView::updatePreparingTile(vector<Tile> &tiles) {
	if (tiles.size() != 4) {
		throw Exception("Preparing tiles should have size 4!");
	}
	for (size_t i = 0; i < tiles.size(); ++i) {
		copyFullTileToRenderer(&tiles[i], RECT_PREPARING[i]);
	}
	SDL_RenderPresent(renderer.get());
}

shared_ptr<SDL_Window> GameView::getWindow() {
	return window;
}