#include <iostream>
#include <SDL.h>
#include <memory>
#pragma once

using namespace std;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

class Game {
public:
  Game();
  virtual ~Game();

  static void start();
};