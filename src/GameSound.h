#pragma once
#include <SDL_mixer.h>
#include <memory>
#include "PointerDefinition.h"

using namespace std;

class GameSound {
private:
    static shared_ptr<GameSound> instance;

    shared_ptr<Mix_Music> background;
    shared_ptr<Mix_Chunk> moveLeftRight;
    shared_ptr<Mix_Chunk> lineClear;
    shared_ptr<Mix_Chunk> hardDrop;

public:
    static shared_ptr<GameSound> getInstance();
    GameSound();
    virtual ~GameSound();

    void initSound();
    void playBackground();
    void playMoveLeftRight();
    void playLineClear();
    void playHardDrop();
};