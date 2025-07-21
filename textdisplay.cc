#include "textdisplay.h"

TextDisplay::TextDisplay(Board* b, std::size_t size = 8): 
        theTextDisplay(size, std::vector<char>(size, ' ')), board{b} {};