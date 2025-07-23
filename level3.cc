#include "level3.h"
#include "move.h"
#include "board.h"
#include <random>

// Helper func
bool isInVector(const std::vector<Posn>& list, const Posn& p) {
    for (const Posn& q : list) {
        if (q.row == p.row && q.col == p.col) {
            return true;
        }
    }
    return false;
}

Move Level3::chooseMove(Board& board, Colour colour) {

    // Get all valid moves
    std::vector<Move> allMoves = board.legalMoves(colour);
    int numMoves = allMoves.size();

    std::vector<Move> retreatMoves;
    std::vector<Move> captureMoves;

    // Collect all Capture moves
    for (int i = 0; i < numMoves; i++) {
        Posn target = allMoves[i].to;
        Piece* targetPiece = board.getPieceAt(target.row, target.col);
        if (targetPiece != nullptr && targetPiece->getColour() != colour) {
            captureMoves.push_back(allMoves[i]);
        }
    }

    // Collect all Threatened Grids
    Colour opponent = (colour == Colour::White) ? Colour::Black : Colour::White;
    std::vector<Move> opponentMoves = board.legalMoves(opponent);

    std::vector<Posn> threatenedSquares;
    for (const Move& mv : opponentMoves) {
        if (!isInVector(threatenedSquares, mv.to)) {
            threatenedSquares.push_back(mv.to);
        }
    }

    // Collect all Retreat moves (from threatened to safe)
    for (const Move& mv : allMoves) {
        if (isInVector(threatenedSquares, mv.from) &&
            !isInVector(threatenedSquares, mv.to)) {
            retreatMoves.push_back(mv);
        }
    }

    // Check for Retreat + Capture moves
    for (const Move& mv : retreatMoves) {
        for (const Move& cap : captureMoves) {
            if (mv.from.row == cap.from.row && mv.from.col == cap.from.col &&
                mv.to.row == cap.to.row && mv.to.col == cap.to.col) {
                return mv;
            }
        }
    }

    // Prefer Retreat
    if (!retreatMoves.empty()) {
        static bool seeded = false;
        if (!seeded) {
            std::srand(std::time(nullptr));
            seeded = true;
        }
        int index = std::rand() % retreatMoves.size();
        return retreatMoves[index];
    }

    // Then Capture
    if (!captureMoves.empty()) {
        static bool seeded = false;
        if (!seeded) {
            std::srand(std::time(nullptr));
            seeded = true;
        }
        int index = std::rand() % captureMoves.size();
        return captureMoves[index];
    }

    // No better strategy, back to random legal move
    static bool seeded = false;
    if (!seeded) {
        std::srand(std::time(nullptr));
        seeded = true;
    }
    int index = std::rand() % numMoves;

    return allMoves[index];
}
