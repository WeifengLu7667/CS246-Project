#ifndef COMPUTERPLAYER_H
#define COMPUTERPLAYER_H

#include "player.h"
#include "board.h"
#include "move.h"

class ComputerPlayer: public Player {
public:
    Move makeMove(Board &board) override;

};

#endif
