#ifndef PIECE_H
#define PIECE_H

#include <vector>
#include "board.h"
#include "posn.h"
#include "colour.h"

using namespace std;

class Piece {
protected:
    Colour colour;
    bool moved;

public:
    // Ctor
    Piece(Colour c) : colour{c}, moved{false} {}
    // Dtor
    virtual ~Piece() = default;

    Colour getColour() const;
    bool hasMoved() const;
    void setMoved(bool m);

    // Pure virtual
    virtual char getSymbol() const = 0;
    virtual std::vector<Posn> getValidMoves(const Board& b, Posn p) const = 0;
};



#endif
