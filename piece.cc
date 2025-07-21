#include "piece.h"

Colour Piece::getColour() const {
    return colour;
}

bool Piece::hasMoved() const {
    return moved;
}

void Piece::setMoved(bool m) {
    moved = m;
}
