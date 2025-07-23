#include "level1.h";
#include <random>

Move Level1::chooseMove(Board& board, Colour colour) {
    std::vector<Move> allMoves = board.legalMoves(colour);

}
