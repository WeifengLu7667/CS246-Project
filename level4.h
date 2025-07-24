#ifndef LEVEL4_H
#define LEVEL4_H

#include "strategy.h"
#include "board.h"
#include "move.h"

// Prefer Retreat (avoid being captured) > Capture (High Value > Low Value) > Random Legal Moves
class Level4: public Strategy {
    public:
        Move chooseMove(Board &board, Colour colour) override;
};

#endif