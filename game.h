#ifndef GAME_H
#define GAME_H

#include "board.h"
#include "scoreboard.h"
#include "player.h"
#include "move.h"
#include <stack>

class Game {
    Board board;
    Scoreboard scoreboard;
    unique_ptr<Player> whitePlayer;
    unique_ptr<Player> blackPlayer;
    std::stack<State> History;
    bool isRunning; // true if the game is running, false if the game is over
    bool useDefaultBoard; // false if the user uses setup mode.
    Posn convertToPosn(const std::string& posnStr);
    void displayBoard(const std::vector<std::vector<char>>& setupBoard);

public:
    void gameRun();
};

#endif