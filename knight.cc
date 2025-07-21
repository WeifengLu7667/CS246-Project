#include "knight.h"
#include "board.h"

Knight::Knight(Colour c) : Piece{c} {}

char Knight::getSymbol() const {
    return colour == Colour::White ? 'N' : 'n';
}

std::vector<Posn> Knight::getValidMoves(const Board& b, Posn p) const {
    std::vector<Posn> validMoves;

    const int dx[] = {-2, -1, 1, 2, 2, 1, -1, -2};
    const int dy[] = {1, 2, 2, 1, -1, -2, -2, -1};



}