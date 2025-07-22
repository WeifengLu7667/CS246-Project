#include "board.h"
#include "piece.h"
#include "move.h"

#include <iostream>
#include <vector>
#include <algorithm>

Board::Board(std::size_t gridSize): 
        board(gridSize, Row(gridSize)), gridSize{gridSize} {};

bool Board::movePiece(const Move &m) {
        Posn start = m.from;
        Posn end = m.to;

        auto &src = board[start.row][start.col];
        auto &des = board[end.row][end.col];

        if (!src) return false;

        const auto moves = src->getValidMoves(*this, start);
        if (std::find(moves.begin(), moves.end(), des) 
                == moves.end()) return false;

        des = std::move(src);
        notifyObservers();
        return true;
}

Piece* Board::getPieceAt(std::size_t x, std::size_t y) const {
        return board[x][y].get();
}

bool Board::isCheck(Colour c) const {
        char king = (c == Colour::White) ? 'K' : 'k';
        
        for (auto &row : board) {
                for (auto &piecePtr : row) {
                        if (piecePtr)
                }
        }
}

