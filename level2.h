#ifndef LEVEL2_H
#define LEVEL2_H

#include "strategy.h"
#include "board.h"
#include "move.h"

// Prefer capturing moves and checks over other moves
class Level2: public Strategy {
    public:
        Move chooseMove(Board &board, Colour colour) override;
};

#endif