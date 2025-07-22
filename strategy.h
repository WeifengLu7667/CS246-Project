#ifndef STRATEGY_H
#define STRATEGY_H

#include "board.h"

class Strategy {
    public:
        virtual Move chooseMove(Board &board) = 0;
};

#endif
