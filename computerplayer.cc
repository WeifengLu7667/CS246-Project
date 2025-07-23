#include "computerplayer.h"

Move ComputerPlayer::makeMove(Board &board) {
    return strategy->chooseMove(board, this->getColour());
}
