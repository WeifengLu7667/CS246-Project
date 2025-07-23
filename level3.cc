#include "level3.h"
#include "move.h"
#include "board.h"
#include <random>

Move Level3::chooseMove(Board& board, Colour colour) {
    std::vector<Move> allMoves = board.legalMoves(colour);

    int numMoves = allMoves.size();
    // 



    // No better strategy, back to random legal move
    static bool seeded = false;
    if (!seeded) {
        std::srand(std::time(nullptr));
        seeded = true;
    }
    int index = std::rand() % numMoves;

    return allMoves[index];
}