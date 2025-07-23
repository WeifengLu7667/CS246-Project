#include "level1.h"
#include "move.h"
#include "board.h"
#include <random>

Move Level1::chooseMove(Board& board, Colour colour) {
    std::vector<Move> allMoves = board.legalMoves(colour);

    int numMoves = allMoves.size();

    // Generate a random number within the range
    static bool seeded = false;
    if (!seeded) {
        std::srand(std::time(nullptr));
        seeded = true;
    }
    int index = std::rand() % numMoves;

    return allMoves[index];
}

/*
Level 1 strategy support:
    Random legal moves
*/
