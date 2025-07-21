#ifndef PIECE_H
#define PIECE_H

#include <vector>
#include "posn.h"
using namespace std;

class Piece {
protected:
    Posn posn;
    char symbol;
    int color;

public:
    // ctor
    Piece(int row, int col, char symbol, int color);



};







#endif
