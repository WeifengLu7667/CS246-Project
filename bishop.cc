#include "bishop.h"
#include "board.h"

Bishop::Bishop(Colour c) : Piece{c} {}

char Bishop::getSymbol() const {
    return colour == Colour::White ? 'B' : 'b';
}

std::vector<Posn> Bishop::getValidMoves(const Board& b, Posn p) const {
    std::vector<Posn> validMoves;

}
