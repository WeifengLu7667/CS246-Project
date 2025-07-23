#ifndef ROOK_H
#define ROOK_H

#include "piece.h"
#include "posn.h"

class Rook : public Piece {
public:
    Rook(Colour c);

    char getSymbol() const override;
    std::unique_ptr<Piece> clone() const override;
    std::vector<Posn> getValidMoves(const Board& b, Posn p) const override;
};

#endif
