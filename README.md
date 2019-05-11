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
All functions have descriptive names and easy to understand themselves. Therefore the below section will describe the `responsibility` of a specific class. [Single Responsibility Principle](https://en.wikipedia.org/wiki/Single_responsibility_principle)

### `Controller` classes

+ `GameSound`
  + Manage sound such as background, moving,...
+ `GameController`
  + TODO

### `View` classes

+ `GameView`
  + `GameView` is responsible for rendering `Model` object into screen
+ `Scene`
  + Base class for all scenes in the game
  + Has the implementation of adding custom buttons
  + Responsible to handling or passing event to its inheritance.
  + Somewhat implement crude `life cycle` (`gameLoop` and `redraw`)
+ `SceneStart`
  + TODO

### `Model` classes

+ Model

`Game` class is responsible for receiving `SDL_Event` and then handle such as changing `Scene` or passing it to the current `Scene`  