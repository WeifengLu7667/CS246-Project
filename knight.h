#ifndef KNIGHT_H
#define KNIGHT_H

#include "piece.h"
#include "posn.h"

class Knight : public Piece {
public:
    // ctor
    Knight(Colour c);

    char getSymbol() const override;
    std::vector<Posn> getValidMoves(const Board& b, Posn p) const override;
};

#endif
