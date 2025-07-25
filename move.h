#ifndef MOVE_H
#define MOVE_H

#include "posn.h"

struct Move {
    Posn from;
    Posn to;
    char promo; // ' ' represents no promotion
};

#endif
