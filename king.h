#ifndef KING_H
#define KING_H

#include "piece.h"
#include "posn.h"

class King : public Piece {
public:
    King(Colour c);

    char getSymbol() const override;
    std::vector<Posn> getValidMoves(const Board& b, Posn p) const override;
};

#endif
