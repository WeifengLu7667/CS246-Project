#ifndef TEXTDISPLAY_H
#define TEXTDISPLAY_H

#include "observer.h"
#include "board.h"
#include <iostream>
#include <vector>

class TextDisplay: public Observer {
    std::vector<std::vector<char>> theTextDisplay;
    Board *board;

    public:
        explicit TextDisplay(Board* b, std::size_t size = 8);

        void notify(Subject &whoNotified) override;
        friend std::ostream &operator<<(std::ostream &out, const TextDisplay &td);
};

std::ostream &operator<<(std::ostream &out, const TextDisplay &td);

#endif