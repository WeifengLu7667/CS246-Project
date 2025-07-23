#ifndef LEVEL3_H
#define LEVEL3_H

#include "strategy.h"
#include "board.h"
#include "move.h"

// Prefer Retreat (avoid being captured) > Capture > Random Legal Moves
class Level3: public Strategy {
    public:
        Move chooseMove(Board &board, Colour colour) override;
};

#endif