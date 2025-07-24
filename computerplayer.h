#ifndef COMPUTERPLAYER_H
#define COMPUTERPLAYER_H

#include <memory>
#include "player.h"
#include "board.h"
#include "move.h"
#include "strategy.h"

class ComputerPlayer: public Player {
    std::unique_ptr<Strategy> strategy;
public:
    ComputerPlayer(std::unique_ptr<Strategy> strategy, Colour colour);
    bool isValidCommand(const std::string& line) override;
    Move makeMove(Board &board, const std::string& line) override;
};

#endif
