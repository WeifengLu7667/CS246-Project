#include "knight.h"
#include "board.h"

Knight::Knight(Colour c) : Piece{c} {}

char Knight::getSymbol() const {
    return colour == Colour::White ? 'N' : 'n';
}

std::vector<Posn> Knight::getValidMoves(const Board& b, Posn p) const {
    std::vector<Posn> validMoves;

    const int dx[] = {-2, -1, 1, 2, 2, 1, -1, -2};
    const int dy[] = {1, 2, 2, 1, -1, -2, -2, -1};

    for (int i = 0; i < 8; i++) {
        int newRow = p.row + dx[i];
        int newCol = p.col + dy[i];

        if (newRow >= 0 && newRow <=7 && newCol >= 0 && newCol <=7) {
            Piece* dest = getPieceAt(newRow, newCol);
            if (dest == nullptr || dest->getColour() != this->getColour()) {
                validMoves.emplace_back(Posn{newRow, newCol});
            }
        }
    }
    return validMoves;
}