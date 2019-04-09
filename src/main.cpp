#include <iostream>
#include <SDL.h>
#include "Game.h"
#ifdef TEST
#include "Test.h"
#endif

int main( __attribute__((unused)) int argc, __attribute__((unused)) char **argv) {
#ifdef TEST
	Test::runAllTest();
#else
	Game::getInstance()->start();
#endif
	return 0;
}