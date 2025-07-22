#ifndef BOARD_H
#define BOARD_H

#include "subject.h"
#include "piece.h"
#include "state.h"
#include "colour.h"
#include "move.h"
#include <vector>
#include <memory>

class Piece;

class Board: public Subject {
    using Row = vector<std::unique_ptr<Piece>>;
    std::vector<Row> board;
    std::size_t gridSize;

    State state;

public:
    // Big Five
    explicit Board(std::size_t gridSize = 8);

    Board(const Board&other);

    Board(Board&&other);

    Board &operator=(const Board&other);

    Board &operator=(Board &&other);

    ~Board() = default;

    // Public methods

    // returns the movement is valid or not. if true, the movement is made. 
    // if false, the movement is not made.
    bool movePiece(const Move &m);

    // return the raw pointer of the Piece at (x, y) without ownership transfer
    Piece* getPieceAt(std::size_t x, std::size_t y) const;

    bool isCheck(Colour c) const; // check using the actual board (check the positions of the pieces), not simply check the state. Also, change the state.status if the state doesn't match the result.

    bool isCheckMate(Colour c) const; // same requirementsas isCheck

    bool isStaleMate(Colour c) const; // same requirements as isCheck

    void setupDefaultBoard();

    // accessor to private field State
    State getGameState() const;

    // optional
    void setupFromFen(const std::string& s);

    // return the possible move
    std::vector<Move> legalMoves(Colour) const;

    void changeState(State newState);

    void placePiece(char sym, Posn p);

    void removePiece(Posn p);

};

#endif
