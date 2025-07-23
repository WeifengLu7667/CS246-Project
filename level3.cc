#include "level3.h"
#include "move.h"
#include "board.h"
#include <random>

Move Level3::chooseMove(Board& board, Colour colour) {
    std::vector<Move> allMoves = board.legalMoves(colour);

}