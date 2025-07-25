#ifndef LEVEL5_H
#define LEVEL5_H

#include "strategy.h"
#include "board.h"
#include "move.h"

// Prefer Retreat (avoid being captured) > Capture > Random Legal Moves
// Now prioritize High value pieces in both Retreat and Capture
class Level5: public Strategy {
    public:
        Level5();
        Move chooseMove(Board &board, Colour colour) override;
};

#endif
