#ifndef COMPUTERPLAYER_H
#define COMPUTERPLAYER_H

#include "player.h"
#include "board.h"


class ComputerPlayer: public Player {
public:
    void makeMove(Board &board) override;

};

#endif
