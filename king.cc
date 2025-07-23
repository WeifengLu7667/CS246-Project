#include "king.h"
#include "board.h"

King::King(Colour c) : Piece{c} {}

std::unique_ptr<Piece> King::clone() const { return std::make_unique<King>(*this);}

char King::getSymbol() const {
    return colour == Colour::White ? 'K' : 'k';
}

std::vector<Posn> King::getValidMoves(const Board& b, Posn p) const {
    std::vector<Posn> validMoves;

    const int dx[] = {-1, -1, -1, 0, 0, 1, 1, 1};
    const int dy[] = {-1, 0, 1, -1, 1, -1, 0, 1};

    for (int i = 0; i < 8; i++) {
        int newRow = p.row + dx[i];
        int newCol = p.col + dy[i];

        if (newRow >= 0 && newRow <= 7 && newCol >= 0 && newCol <= 7) {
            Piece* dest = b.getPieceAt(newRow, newCol);
            if (dest == nullptr || dest->getColour() != this->getColour()) {
                validMoves.emplace_back(Posn{newRow, newCol});
            }
        }
    }
    
    return validMoves;
}
