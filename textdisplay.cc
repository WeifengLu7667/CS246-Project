#include "textdisplay.h"
#include "board.h"
#include "piece.h"

#include <iostream>

TextDisplay::TextDisplay(shared_ptr<Board> b, std::size_t size = 8): 
        theTextDisplay(size, std::vector<char>(size, ' ')), board{b} {
            b->attach(this);
        };

void TextDisplay::notify(Subject &whoNotified) {
    
}