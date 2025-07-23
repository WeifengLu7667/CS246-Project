#ifndef KNIGHT_H
#define KNIGHT_H

#include "piece.h"
#include "posn.h"

class Knight : public Piece {
public:
    Knight(Colour c);

    char getSymbol() const override;
    std::unique_ptr<Piece> clone() const override;
    std::vector<Posn> getValidMoves(const Board& b, Posn p) const override;
};

#endif
