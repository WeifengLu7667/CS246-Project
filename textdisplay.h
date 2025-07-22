#ifndef TEXTDISPLAY_H
#define TEXTDISPLAY_H

#include "observer.h"
#include "board.h"
#include "state.h"

#include <iostream>
#include <vector>
#include <memory>

class TextDisplay: public Observer {
    std::vector<std::vector<char>> theTextDisplay;
    Board *b;

public:
    explicit TextDisplay(Board *b, std::size_t size = 8);

    void notify() override;

    friend std::ostream &operator<<(std::ostream &out, const TextDisplay &td);
};

std::ostream &operator<<(std::ostream &out, const TextDisplay &td);

#endif
