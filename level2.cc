#include "level2.h"
#include "move.h"
#include "board.h"
#include <random>

Move Level2::chooseMove(Board& board, Colour colour) {

    // Get all valid moves
    std::vector<Move> allMoves = board.legalMoves(colour);
    int numMoves = allMoves.size();

    // Try capture
    for (int i = 0; i < numMoves; i++) {
        Posn target = allMoves[i].to;
        Piece* targetPiece = board.getPieceAt(target.row, target.col);
        if (targetPiece != nullptr && targetPiece->getColour() != colour) {
            return allMoves[i];
        }
    }

    // No capture possible, randomly pick a move
    static bool seeded = false;
    if (!seeded) {
        std::srand(std::time(nullptr));
        seeded = true;
    }
    int index = std::rand() % numMoves;

    return allMoves[index];
}

/*
Feature TO BE ADDED:
    Prefer Check > normal Cpature
    Prefer Capture Higher-value pieces > Capture Lower-value pieces
*/
