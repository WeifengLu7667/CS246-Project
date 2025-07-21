#ifndef PAWN_H
#define PAWN_H

#include "piece.h"
#include "posn.h"

class Pawn : public Piece {
public:
    Pawn(Colour c);

    char getSymbol() const override;
    std::vector<Posn> getValidMoves(const Board& b, Posn p) const override;
};

#endif
