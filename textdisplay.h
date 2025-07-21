#ifndef TEXTDISPLAY_H
#define TEXTDISPLAY_H

#include "observer.h"
#include <vector>

class TextDisplay: public Observer {
    const std::vector<std::vector<char>> theTextDisplay;
    const static size_t len = 8;
    public:
        void notify(Subject &whoNotified) override;

};

#endif