#ifndef TEXTDISPLAY_H
#define TEXTDISPLAY_H

#include "observer.h"
#include "board.h"

#include <iostream>
#include <vector>
#include <memory>

class TextDisplay: public Observer {
    std::vector<std::vector<char>> theTextDisplay;
    shared_ptr<Board> board;

public:
    explicit TextDisplay(shared_ptr<Board> b, std::size_t size = 8);

    void notify(Subject &whoNotified) override;

    friend std::ostream &operator<<(std::ostream &out, const TextDisplay &td);
};

std::ostream &operator<<(std::ostream &out, const TextDisplay &td);

#endif