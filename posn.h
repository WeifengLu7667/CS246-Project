#ifndef POSN_H
#define POSN_H

struct Posn {
    int row;
    int col;

    bool operator==(const Posn& other) const {
        return row == other.row && col == other.col;
    }

    bool operator!=(const Posn& other) const {
        return !(*this == other);
    }
};

#endif
