#include "level2.h"
#include "move.h"
#include "board.h"
#include <random>

#include "level2.h"
#include "board.h"
#include "move.h"
#include <vector>
#include <random>
#include <ctime>

Move Level2::chooseMove(Board& board, Colour colour) {
    std::vector<Move> allMoves = board.legalMoves(colour);
    Colour opponent = (colour == Colour::White) ? Colour::Black : Colour::White;

    std::vector<Move> mateMoves;
    std::vector<Move> checkMoves;
    std::vector<Move> captureMoves;

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

    static bool seeded = false;
    if (!seeded) {
        std::srand(std::time(nullptr));
        seeded = true;
    }

    if (!mateMoves.empty()) {
        return mateMoves[std::rand() % mateMoves.size()];
    }
    if (!checkMoves.empty()) {
        return checkMoves[std::rand() % checkMoves.size()];
    }
    if (!captureMoves.empty()) {
        return captureMoves[std::rand() % captureMoves.size()];
    }

    return allMoves[std::rand() % allMoves.size()];
}
