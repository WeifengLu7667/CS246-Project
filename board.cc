#include <iostream>
#include "board.h"

Board::Board(std::size_t gridSize): 
        board(gridSize, Row(gridSize)), gridSize{gridSize} {};
