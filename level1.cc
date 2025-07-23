#include "level1.h"
#include "move.h"
#include "board.h"
#include <random>

Move Level1::chooseMove(Board& board, Colour colour) {

    // Get all valid moves
    std::vector<Move> allMoves = board.legalMoves(colour);
    int numMoves = allMoves.size();

    // Randomly pick a move
    static bool seeded = false;
    if (!seeded) {
        std::srand(std::time(nullptr));
        seeded = true;
    }
    int index = std::rand() % numMoves;

    return allMoves[index];
}
