#pragma once
#include <memory>
#include <map>
#include "Board.h"
using namespace std;

/*
    usage GameView::getInstance()->updateBoard(const Board &board);
*/
class GameView {
private:
    static shared_ptr<GameView> instance;
public:
    static shared_ptr<GameView> getInstance();
    GameView();
    virtual ~GameView();
    void updateBoard(const Board &board);
};