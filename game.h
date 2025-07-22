#ifndef GAME_H
#define GAME_H

#include "board.h"
#include "player.h"
#include "move.h"
#include <stack>

class Game {
    Board board;
    unique_ptr<Player> whitePlayer;
    unique_ptr<Player> blackPlayer;
    std::stack<State> History;
    Posn convertToPosn(const std::string& posnStr);
    void displayBoard(const std::vector<std::vector<char>>& setupBoard);

public:
    void gameRun();
};

#endif