#include "level4.h"
#include "move.h"
#include "board.h"
#include <random>
#include <ctime>

int getPieceValue(char symbol) {
    switch (symbol) {
        case 'Q': case 'q': return 9;
        case 'R': case 'r': return 5;
        case 'B': case 'b': return 3;
        case 'N': case 'n': return 3;
        case 'P': case 'p': return 1;
        default: return 0;
    }
}

