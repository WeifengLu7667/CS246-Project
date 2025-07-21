#ifndef BOARD_H
#define BOARD_H

#include "subject.h"
#include <vector>
#include <memory>


class Board: public Subject {
    std::vector<std::unique_ptr<Piece>>
};

#endif
