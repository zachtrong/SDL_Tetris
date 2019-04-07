#include <iostream>
#include <SDL.h>
#include "Game.h"

int main( __attribute__((unused)) int argc, __attribute__((unused)) char **argv) {
	Game::getInstance()->start();
	return 0;
}