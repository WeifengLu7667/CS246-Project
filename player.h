#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include "board.h"
#include "move.h"

class Player {
    std::string name;
    

public:
    virtual void makeMove(Board &board, Move &move) = 0;
    std::string getName() const;
};  

#endif