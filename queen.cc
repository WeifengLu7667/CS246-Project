#include "queen.h"
#include "board.h"

Queen::Queen(Colour c) : Piece{c} {}

char Queen::getSymbol() const {
    return colour == Colour::White ? 'Q' : 'q';
}

std::vector<Posn> Queen::getValidMoves(const Board& b, Posn p) const {
    std::vector<Posn> validMoves;

    const int dx[] = {-1, -1, -1, 0, 0, 1, 1, 1};
    const int dy[] = {-1, 0, 1, -1, 1, -1, 0, 1};

    for (int i = 0; i < 8; i++) {
        int newRow = p.row + dx[i];
        int newCol = p.col + dy[i];

        while (newRow >= 0 && newRow <= 7 && newCol >= 0 && newCol <= 7) {
            Piece* dest = b.getPieceAt(newRow, newCol).get();
            if (dest == nullptr) {
                validMoves.emplace_back(Posn{newRow, newCol});
            } else {
                if (dest->getColour() != this->getColour()) {
                    validMoves.emplace_back(Posn{newRow, newCol});
                }
                break;
            }
            newRow += dx[i];
            newCol += dy[i];
        }
    }
    return validMoves;
}
