#include "level3.h"
#include "move.h"
#include "board.h"
#include <random>
#include <ctime>

// Helper
bool isInVector(const std::vector<Posn>& list, const Posn& p) {
    for (const Posn& q : list) {
        if (q.row == p.row && q.col == p.col) {
            return true;
        }
    }
    return false;
}

Move Level3::chooseMove(Board& board, Colour colour) {
    std::vector<Move> allMoves = board.legalMoves(colour);
    Colour opponent = (colour == Colour::White) ? Colour::Black : Colour::White;

    std::vector<Move> mateMoves;
    std::vector<Move> checkMoves;
    std::vector<Move> retreatMoves;
    std::vector<Move> captureMoves;

    // Collect checkmate & check & capture moves
    for (const Move& mv : allMoves) {
        Board copy = board;
        copy.movePiece(mv);

        if (copy.isCheckMate(opponent)) {
            mateMoves.push_back(mv);
        } else if (copy.isCheck(opponent)) {
            checkMoves.push_back(mv);
        } else {
            Piece* target = board.getPieceAt(mv.to.row, mv.to.col);
            if (target != nullptr && target->getColour() != colour) {
                captureMoves.push_back(mv);
            }
        }
    }

    // Identify threatened squares
    std::vector<Move> opponentMoves = board.legalMoves(opponent);
    std::vector<Posn> threatenedSquares;
    for (const Move& mv : opponentMoves) {
        if (!isInVector(threatenedSquares, mv.to)) {
            threatenedSquares.push_back(mv.to);
        }
    }

    // Collect Retreat moves
    for (const Move& mv : allMoves) {
        if (isInVector(threatenedSquares, mv.from) &&
            !isInVector(threatenedSquares, mv.to)) {
            retreatMoves.push_back(mv);
        }
    }

    // Random seeding
    static bool seeded = false;
    if (!seeded) {
        std::srand(std::time(nullptr));
        seeded = true;
    }

    // Pick move: mate > check > retreat > capture > random
    if (!mateMoves.empty()) {
        return mateMoves[std::rand() % mateMoves.size()];
    }
    if (!checkMoves.empty()) {
        return checkMoves[std::rand() % checkMoves.size()];
    }
    if (!retreatMoves.empty()) {
        return retreatMoves[std::rand() % retreatMoves.size()];
    }
    if (!captureMoves.empty()) {
        return captureMoves[std::rand() % captureMoves.size()];
    }

    return allMoves[std::rand() % allMoves.size()];
}
