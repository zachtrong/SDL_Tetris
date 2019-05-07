# Tetris

Another tetris game reconstructed by `zimportant` and `nguyenthanhvinh` for the sake of entertainment :)

## Building

1. `scripts/run`
Debugging purpose (build then run immediately)
2. `scripts/build.bat`
Build debug application into `build/` directory
3. `scripts/release.bat`
Build release application into `release/` directory
4. `scripts/test.bat`
Run some basic tests of implementation

## Program flow

This game uses [MVC Architecture](https://en.wikipedia.org/wiki/Model%E2%80%93view%E2%80%93controller) in a basic and crude way.  

### `Controller` classes

+ GameSound
+ GameController

### `View` classes

+ GameView
+ Scene ans its descendent

### `Model` classes

+ Model

`Game` class is responsible for receiving `SDL_Event` and then handle such as change `Scene` or pass it to the current `Scene`  