#include "board.h"
#include "piece.h"
#include "move.h"

#include <iostream>
#include <vector>
#include <algorithm>

Board::Board(std::size_t gridSize): 
	board(gridSize, Row(gridSize)), gridSize{gridSize} {};

bool Board::movePiece(const Move &m) {
	Posn start = m.from;
	Posn end = m.to;

	auto &src = board[start.row][start.col];
	auto &des = board[end.row][end.col];

	if (!src) return false;

	const auto moves = src->getValidMoves(*this, start);
	if (std::find(moves.begin(), moves.end(), des) == moves.end()) return false;

	des = std::move(src);
	notifyObservers();
	
	return true;
}

Piece* Board::getPieceAt(std::size_t x, std::size_t y) const {
	return board[x][y].get();
}

bool Board::isCheck(Colour c) const {
	char kingSymbol = (c == Colour::White) ? 'K' : 'k';
	Posn kingPos = {-1, -1};
	Posn piecePos = {-1, -1};

	for (std::size_t row = 0; row < gridSize; ++row) {
		for (std::size_t col = 0; col < gridSize; ++ col) {
			if (board[row][col] && board[row][col]->getSymbol() == kingSymbol) {
				kingPos = {static_cast<int>(row), static_cast<int>(col)};
				break;
			}
		}
	}

	Colour enemyColour = (c == Colour::White) ? Colour::Black : Colour::White;

	for (std::size_t row = 0; row < gridSize; ++row) {
		for (std::size_t col = 0; col < gridSize; ++ col) {
			const auto &piece = board[row][col];
			piecePos = {static_cast<int>(row), static_cast<int>(col)};

			if (piece->getColour() != enemyColour) continue;
				
			const auto moves = piece->getValidMoves(*this, piecePos);
			if (std::find(moves.begin(), moves.end(), kingPos) != moves.end()) {
				return true;
			}
		}
	}
	return false;

}

