#include "bishop.h"
#include "board.h"

Bishop::Bishop(Colour c) : Piece{c} {}

char Bishop::getSymbol() const {
    return colour == Colour::White ? 'B' : 'b';
}

std::vector<Posn> Bishop::getValidMoves(const Board& b, Posn p) const {
    std::vector<Posn> moves;

    const int dx[] = {-1, -1, 1, 1};
    const int dy[] = {-1, 1, -1, 1};

    for (int dir = 0; dir < 4; dir++) {
        int r = p.row + dx[dir];
        int c = p.col + dy[dir];

        while (r >= 0 && r < 8 && c >= 0 && c < 8) {
            Piece* dest = b.getPieceAt(r, c);
            if (!dest) {
                moves.emplace_back(Posn{r, c});
            } else {
                if (dest->getColour() != this->getColour()) {
                    moves.emplace_back(Posn{r, c}); // capture
                }
                break; // blocked
            }
            r += dx[dir];
            c += dy[dir];
        }
    }

    return moves;
}
