#include "GameSound.h"

shared_ptr<GameSound> GameSound::instance(nullptr);

shared_ptr<GameSound> GameSound::getInstance() {
    if (instance == nullptr) {
        instance = make_shared<GameSound>();
    }
    return instance;
}

GameSound::GameSound()
    :background(nullptr),
    moveLeftRight(nullptr),
    lineClear(nullptr),
    hardDrop(nullptr)
{
}

GameSound::~GameSound() {
    Mix_Quit();
}

void GameSound::initSound() {
    Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 );

    background = PointerDefinition::createMixMusic(
        Mix_LoadMUS("assets/sound_effects/background.wav")
    );
    moveLeftRight = PointerDefinition::createMixChunk(
        Mix_LoadWAV("assets/sound_effects/move_left_right.wav")
    );
    lineClear = PointerDefinition::createMixChunk(
        Mix_LoadWAV("assets/sound_effects/line_clear.wav")
    );
    hardDrop = PointerDefinition::createMixChunk(
        Mix_LoadWAV("assets/sound_effects/hard_drop.wav")
    );

    playBackground();
}

void GameSound::playBackground() {
    if (Mix_PlayingMusic() == 0) {
        Mix_PlayMusic(background.get(), -1);
    }
}

void GameSound::playMoveLeftRight() {
    Mix_PlayChannel(-1, moveLeftRight.get(), 0);
}

void GameSound::playLineClear() {
    Mix_PlayChannel(-1, lineClear.get(), 0);
}

void GameSound::playHardDrop() {
    Mix_PlayChannel(-1, hardDrop.get(), 0);
}