#ifndef LEVEL4_H
#define LEVEL4_H

#include "strategy.h"
#include "board.h"
#include "move.h"

// Prefer Retreat (avoid being captured) > Capture > Random Legal Moves
// Now prioritize High value pieces in both Retreat and Capture
class Level4: public Strategy {
    public:
        Move chooseMove(Board &board, Colour colour) override;
};

#endif
