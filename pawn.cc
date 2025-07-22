#include "pawn.h"
#include "board.h"

Pawn::Pawn(Colour c) : Piece{c} {}

char Pawn::getSymbol() const {
    return colour == Colour::White ? 'P' : 'p';
}

std::vector<Posn> Pawn::getValidMoves(const Board& b, Posn p) const {
    std::vector<Posn> validMoves;



}