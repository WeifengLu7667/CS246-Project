#ifndef LEVEL1_H
#define LEVEL1_H

#include "strategy.h"
#include "board.h"
#include "move.h"

class Level1: public Strategy {

    public:
        Move chooseMove(Board &board, Colour colour) override;
};

#endif