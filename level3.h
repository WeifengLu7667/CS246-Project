#ifndef LEVEL3_H
#define LEVEL3_H

#include "strategy.h"
#include "board.h"
#include "move.h"

class Level3: public Strategy {
    public:
        Move chooseMove(Board &board) override;
};

#endif