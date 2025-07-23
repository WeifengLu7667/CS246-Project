#include "textdisplay.h"
#include "board.h"
#include "piece.h"

#include <iostream>
#include <memory>

TextDisplay::TextDisplay(Board *b, std::size_t size): 
        theTextDisplay(size, std::vector<char>(size, ' ')), b{b} {
            b->attach(this);
            notify();
        };

void TextDisplay::notify() {
    const std::size_t n = theTextDisplay.size();

    for (std::size_t r = 0; r < n; ++r) {
        for (std::size_t c = 0; c < n; ++c) {
            const Piece *p = b->getPieceAt(r, c);
            if (p) {
                theTextDisplay[r][c] = p->getSymbol();
            } else {
                bool white_space = ((r + c) % 2 == 0);
                theTextDisplay[r][c] = white_space ? ' ' : '_';
            }
        }
    }
}

std::ostream &operator<<(std::ostream &out, const TextDisplay &td) {
    const std::size_t n = td.theTextDisplay.size();
    for (std::size_t r = 0; r < n; ++r) {
        out << (n - r) << " "; 
        for (char c : td.theTextDisplay[r]) {
            out << c;
        }
        out << std::endl;
    }
    out << std::endl;
    out << "  ";
    for (int f = 'a'; f < 'a' + static_cast<int>(n); ++f) {
        out << static_cast<char>(f);
    }
    out << std::endl;
    
    return out;

}
