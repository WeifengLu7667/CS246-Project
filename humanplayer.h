#ifndef HUMANPLAYER_H
#define HUMANPLAYER_H

#include "player.h"
#include "board.h"
#include "move.h"

class HumanPlayer: public Player {
    Posn convertToPosn(const std::string& posnStr);
    
public:
    bool isValidCommand(const std::string& line) override;
    Move makeMove(Board &board, const std::string& line) override;
};

#endif
