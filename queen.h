#ifndef QUEEN_H
#define QUEEN_H

#include "piece.h"
#include "posn.h"

class Queen : public Piece {
public:
    Queen(Colour c);

    char getSymbol() const override;
    std::unique_ptr<Piece> clone() const override;
    std::vector<Posn> getValidMoves(const Board& b, Posn p) const override;
};

#endif
