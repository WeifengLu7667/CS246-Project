#ifndef GAME_H
#define GAME_H

#include <memory>
#include <stack>
#include "board.h"
#include "scoreboard.h"
#include "player.h"
#include "move.h"
#include "textdisplay.h"

class Game {
    Board board;
    Scoreboard scoreboard;
    std::unique_ptr<Player> whitePlayer;
    std::unique_ptr<Player> blackPlayer;
    std::stack<State> History;
    bool isRunning; // true if the game is running, false if the game is over
    bool useDefaultBoard; // false if the user uses setup mode.
    std::unique_ptr<TextDisplay> textDisplay; // Observer for board display
    Posn convertToPosn(const std::string& posnStr);
    void displayBoard(const std::vector<std::vector<char>>& setupBoard);

public:
    Game();
    void gameRun();
};

#endif