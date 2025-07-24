#include "computerplayer.h"
#include <memory>

ComputerPlayer::ComputerPlayer(std::unique_ptr<Strategy> strategy, Colour colour) : Player(colour), strategy(std::move(strategy)) {}

bool ComputerPlayer::isValidCommand(const std::string& line) {
    // For computer players, the command should be exactly "move"
    return line == "move";
}

Move ComputerPlayer::makeMove(Board &board, const std::string& line) {
    // Computer players ignore the line parameter and use their strategy
    return strategy->chooseMove(board, this->getColour());
}
