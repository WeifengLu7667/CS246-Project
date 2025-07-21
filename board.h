#ifndef BOARD_H
#define BOARD_H

#include "subject.h"
#include "piece.h"
#include "state.h"
#include "colour.h"
#include <vector>
#include <memory>

class Piece;

class Board: public Subject {
    using Row = vector<std::unique_ptr<Piece>>;
    std::vector<Row> board;
    std::size_t gridSize;

    std::unique_ptr<State> state;

    public:
    // Big Five
    explicit Board(std::size_t gridSize = 8) {};

    Board(const Board&other);

    Board(Board&&other);

    Board &operator=(const Board&other);

    Board &operator=(Board &&other);

    ~Board() = default;

    // Public methods

    // returns the movement is valid or not
    bool movePiece(const Move &m);

    // return the raw pointer of the Piece at (x, y) without ownership transfer
    Piece* getPieceAt(int x, int y) const;

    //





};

#endif
