#ifndef HUMANPLAYER_H
#define HUMANPLAYER_H

#include "player.h"
#include "board.h"
#include "move.h"

class HumanPlayer: public Player {
public:
    Move makeMove(Board &board) override;
};

#endif