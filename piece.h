#ifndef PIECE_H
#define PIECE_H

#include <vector>
#include "board.h"
#include "posn.h"
#include "colour.h"

using namespace std;

class Board;


class Piece {
protected:
    Colour colour;
    bool moved;

public:
    Piece(Colour c) : colour{c}, moved{false} {}
    virtual ~Piece() = default;

    Colour getColour() const;
    bool hasMoved() const;
    void setMoved(bool m);

    // Pure virtual
    virtual char getSymbol() const = 0;
    
    virtual std::unique_ptr<Piece> clone() const = 0;

    virtual std::vector<Posn> getValidMoves(const Board& b, Posn p) const = 0; // return the possible positions that the piece can move to
};



#endif
