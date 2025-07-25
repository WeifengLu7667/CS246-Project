#include "level4.h"
#include "move.h"
#include "board.h"
#include <random>
#include <ctime>
#include <vector>
#include <algorithm>

// Define piece value
int getPieceValue(char symbol) {
    switch (symbol) {
        case 'Q': case 'q': return 9;
        case 'R': case 'r': return 5;
        case 'B': case 'b': return 3;
        case 'N': case 'n': return 3;
        case 'P': case 'p': return 1;
        case 'K': case 'k': return 999;
        default: return 0;
    }
}

bool isIn_Vector(const std::vector<Posn>& list, const Posn& p) {
    for (const Posn& q : list) {
        if (q.row == p.row && q.col == p.col) {
            return true;
        }
    }
    return false;
}

// Find best capture
Move findMostValuableCapture(const std::vector<Move>& moves, const Board& board) {
    Move bestMove;
    int maxValue = -1;
    for (const Move& m : moves) {
        Piece* target = board.getPieceAt(m.to.row, m.to.col);
        int value = target ? getPieceValue(target->getSymbol()) : 0;
        if (value > maxValue) {
            maxValue = value;
            bestMove = m;
        }
    }
    return bestMove;
}

// Find best Retreat
Move findMostValuableRetreat(const std::vector<Move>& moves, const Board& board) {
    Move bestMove;
    int maxValue = -1;
    for (const Move& m : moves) {
        Piece* self = board.getPieceAt(m.from.row, m.from.col);
        int value = self ? getPieceValue(self->getSymbol()) : 0;
        if (value > maxValue) {
            maxValue = value;
            bestMove = m;
        }
    }
    return bestMove;
}

Move Level4::chooseMove(Board& board, Colour colour) {
    std::vector<Move> allMoves = board.legalMoves(colour);
    Colour opponent = (colour == Colour::White) ? Colour::Black : Colour::White;

    std::vector<Move> mateMoves;
    std::vector<Move> checkMoves;
    std::vector<Move> retreatMoves;
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
            if (target && target->getColour() != colour) {
                captureMoves.push_back(mv);
            }
        }
    }

    std::vector<Move> opponentMoves = board.legalMoves(opponent);
    std::vector<Posn> threatenedSquares;
    for (const Move& mv : opponentMoves) {
        if (!isIn_Vector(threatenedSquares, mv.to)) {
            threatenedSquares.push_back(mv.to);
        }
    }

    for (const Move& mv : allMoves) {
        if (isIn_Vector(threatenedSquares, mv.from) &&
            !isIn_Vector(threatenedSquares, mv.to)) {
            retreatMoves.push_back(mv);
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
    if (!retreatMoves.empty()) {
        return findMostValuableRetreat(retreatMoves, board);
    }
    if (!captureMoves.empty()) {
        return findMostValuableCapture(captureMoves, board);
    }

    return allMoves[std::rand() % allMoves.size()];
}
