#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include "board.h"
#include "move.h"

class Player {
    std::string name;
    

public:
    std::string getName() const;
    virtual Move makeMove(Board &board) = 0;
};  

#endif