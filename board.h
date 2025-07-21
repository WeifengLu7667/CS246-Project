#ifndef BOARD_H
#define BOARD_H

#include "subject.h"
#include "piece.h"
#include <vector>
#include <memory>


class Board: public Subject {
    std::vector<vector<std::unique_ptr<Piece>>> board;
    public:
    Board() {};
    

};

#endif
