#include "level2.h"
#include "move.h"
#include "board.h"
#include <random>

Move Level2::chooseMove(Board& board, Colour colour) {
    std::vector<Move> allMoves = board.legalMoves(colour);

}