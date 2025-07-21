#include "rook.h"
#include "board.h"

Rook::Rook(Colour c) : Piece{c} {}

char Rook::getSymbol() const {
    return colour == Colour::White ? 'R' : 'r';
}

std::vector<Posn> Rook::getValidMoves(const Board& b, Posn p) const {
    std::vector<Posn> validMoves;

    const int dx[] = {-1, 1, 0, 0};
    const int dy[] = {0, 0, -1, 1};

    for (int i = 0; i < 4; i++) {
        int newRow = p.row + dx[i];
        int newCol = p.col + dy[i];

        while (newRow >= 0 && newRow <= 7 && newCol >= 0 && newCol <=7) {
            Piece* dest = getPieceAt(newRow, newCol);
            if (dest == nullptr) {
                validMoves.emplace_back(Posn{newRow, newCol});
            } else {
                if (dest->getColour() != this->getColour()) {
                    validMoves.emplace_back(Posn{newRow, newCol});
                    break;
                } else {
                    break;
                }
            }
            newRow += dx[i];
            newCol += dy[i];
        }
    }
    return validMoves;
}
