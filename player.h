#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include "board.h"
#include "move.h"

class Player {
    std::string name;
    Colour c;

public:
    std::string getName() const;
    Colour getColour() const;
    virtual bool isValidCommand(const std::string& line) = 0;
    virtual Move makeMove(Board &board, const std::string& line) = 0;
};  

#endif
