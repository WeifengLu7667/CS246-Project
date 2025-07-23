#ifndef BISHOP_H
#define BISHOP_H

#include "piece.h"
#include "posn.h"

class Bishop : public Piece {
public:
    Bishop(Colour c);

    char getSymbol() const override;
    std::unique_ptr<Piece> clone() const override;
    std::vector<Posn> getValidMoves(const Board& b, Posn p) const override;
};

#endif
