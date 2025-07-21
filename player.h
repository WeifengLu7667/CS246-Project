#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include "board.h"


class Player {
    std::string name;
    

public:
    virtual void makeMove(Board &board) = 0;
    std::string getName() const;
};  

#endif