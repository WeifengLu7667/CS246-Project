#include "pawn.h"
#include "board.h"

Pawn::Pawn(Colour c) : Piece{c} {}

std::unique_ptr<Piece> Pawn::clone() const { return std::make_unique<Pawn>(*this); }

char Pawn::getSymbol() const {
    return colour == Colour::White ? 'P' : 'p';
}

std::vector<Posn> Pawn::getValidMoves(const Board& b, Posn p) const {
    std::vector<Posn> validMoves;

    int dir = (colour == Colour::White) ? -1 : 1;
    int startRow = (colour == Colour::White) ? 6: 1;

    int newRow = p.row + dir;
    int newCol = p.col;

    // Move
    if (newRow >= 0 && newRow <= 7 && b.getPieceAt(newRow, newCol) == nullptr) {
        validMoves.emplace_back(Posn{newRow, newCol});
        // First move 2 grids
        if (p.row == startRow && b.getPieceAt(p.row + 2 * dir, newCol) == nullptr) {
            validMoves.emplace_back(Posn{p.row + 2 * dir, newCol});
        }
    }

    // Capture
    Piece* enemy = nullptr;
    int enemyRow = p.col + dir;
    int enemyColLeft = p.col - 1;
    int enemyColRight = p.col + 1;

    // Capture left
    enemy = b.getPieceAt(enemyRow, enemyColLeft);
    if (enemy != nullptr && enemy->getColour() != colour) {
        validMoves.emplace_back(Posn{enemyRow, enemyColLeft});
    }

    // Capture right
    enemy = b.getPieceAt(enemyRow, enemyColRight);
    if (enemy != nullptr && enemy->getColour() != colour) {
        validMoves.emplace_back(Posn{enemyRow, enemyColRight});
    }

    // En Passent
    Posn epTarget1 = {enemyRow, enemyColLeft};
    Posn epTarget2 = {enemyRow, enemyColRight};
    State currState = b.getGameState();
    Posn epTarget = currState.enPassantTarget;
    if (epTarget == epTarget1 || epTarget == epTarget2) {
        validMoves.emplace_back(epTarget);
    }

    return validMoves;
}
