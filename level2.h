#ifndef LEVEL2_H
#define LEVEL2_H

#include "strategy.h"
#include "board.h"
#include "move.h"

class Level2: public Strategy {
    public:
        Move chooseMove(Board &board, Colour colour) override;
};

#endif