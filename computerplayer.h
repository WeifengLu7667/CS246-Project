#ifndef COMPUTERPLAYER_H
#define COMPUTERPLAYER_H

#include "player.h"
#include "board.h"
#include "move.h"
#include "strategy.h"

class ComputerPlayer: public Player {
    unique_ptr<Strategy> strategy;
public:
    Move makeMove(Board &board) override;

};

#endif
