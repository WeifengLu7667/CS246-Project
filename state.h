#ifndef STATE_H
#define STATE_H

#include "colour.h"
#include "castlinginfo.h"
#include "posn.h"
#include <vector>

enum class GameStatus { IN_PROGRESS, WHITE_IN_CHECK, 
    BLACK_IN_CHECK, WHITE_WINS, BLACK_WINS, STALEMATE };

struct State {
    // which turn should the round be
    Colour turn;

    GameStatus status;

    CastlingInfo castlingRights;

    Posn enPassantTarget; // (-1, -1) means no en passant target

    std::vector<std::vector<char>> board; // to access, use board[row][col]
};


#endif