#ifndef BOARD_H
#define BOARD_H

#include "subject.h"
#include "piece.h"
#include <vector>
#include <memory>

class Piece;

class Board: public Subject {
    using Row = vector<std::unique_ptr<Piece>>;
    std::vector<Row> board;
    std::size_t gridSize;

    public:
    explicit Board(std::size_t gridSize = 8) {};



};

#endif
