#include "level3.h"
#include "move.h"
#include "board.h"
#include <random>

Move Level3::chooseMove(Board& board, Colour colour) {

    // Get all valid moves
    std::vector<Move> allMoves = board.legalMoves(colour);
    int numMoves = allMoves.size();

    // Try Escape (avoid being captured)



    // Try Capture



    // Try find both, if not, prefer Esacape, then Capture



    // No better strategy, back to random legal move
    static bool seeded = false;
    if (!seeded) {
        std::srand(std::time(nullptr));
        seeded = true;
    }
    int index = std::rand() % numMoves;

    return allMoves[index];
}

/*
Level 3 strategy supports:
    Prefer Escape (avoid being captured) > Capture > Random Legal Moves
*/