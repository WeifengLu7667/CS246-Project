#ifndef HUMANPLAYER_H
#define HUMANPLAYER_H

#include "player.h"
#include "board.h"

class HumanPlayer: public Player {
public:
    void makeMove(Board &board) override;
};

#endif