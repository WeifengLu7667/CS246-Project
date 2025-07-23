#include "board.h"
#include "piece.h"
#include "pawn.h"
#include "bishop.h"
#include "king.h"
#include "knight.h"
#include "queen.h"
#include "rook.h"
#include "move.h"
#include "textdisplay.h"

#include <iostream>
#include <vector>
#include <algorithm>

// Big 5
Board::Board(std::size_t gridSize): 
	board(gridSize, Row(gridSize)), gridSize{gridSize} {
	// TextDisplay will be attached by the Game controller
}

Board::Board(const Board&other):gridSize{other.gridSize}, 
				board(other.gridSize, Row(other.gridSize)), state{other.state} {
	for (std::size_t r = 0; r < gridSize; ++r) {
		for (std::size_t c = 0; c < gridSize; ++c) {
			if (other.board[r][c]) board[r][c] = other.board[r][c]->clone();
		}
	}
	// TextDisplay will be attached by the Game controller
}


Board &Board::operator=(const Board&other) {
	if (this == &other) return *this;
	gridSize = other.gridSize;
	state = other.state;

	board.assign(gridSize, Row(gridSize));

	for (std::size_t r = 0; r < gridSize; ++r) {
		for (std::size_t c = 0; c < gridSize; ++c) {
			if (other.board[r][c]) board[r][c] = other.board[r][c]->clone();
			else board[r][c].reset();
		}
	}
	// TextDisplay will be attached by the Game controller
	return *this;
}


void Board::snapshotBoard() {
	std::size_t n = gridSize;
    for (std::size_t r = 0; r < n; ++r) {
        for (std::size_t c = 0; c < n; ++c) {
            const auto &piece = board[r][c];
            if (piece) state.board[r][c] = piece->getSymbol();
            else state.board[r][c] = ((r + c) & 1) ? '_' : ' ';
        }
    }
}

bool Board::movePiece(const Move &m) {
	// 0. Sanity Check
	Posn start = m.from;
	Posn end = m.to;

	if (start.row < 0 || start.row >= static_cast<int>(gridSize) ||
		start.col < 0 || start.col >= static_cast<int>(gridSize)) return false;

	auto &src = board[start.row][start.col];
	auto &dst = board[end.row][end.col];

	if (!src) return false;
	Colour c = src->getColour();

	// 1. Legal Move?
	std::vector<Move> all = legalMoves(c);
	bool legal = false;

	for (const Move &lm : all) {
		if (lm.from == m.from && lm.to == m.to && lm.promo == m.promo) {
            legal = true;
            break;
		}
	}

	if (!legal) return false;

	// 1. En Passant Capture BEFORE we move the pawn
	if ((src->getSymbol() == 'P' || src->getSymbol() == 'p') && end == state.enPassantTarget) {
    	int capturedRow = (src->getColour() == Colour::White) ? end.row + 1 : end.row - 1;
    	Posn capturedPosn = Posn{capturedRow, end.col};
    	removePiece(capturedPosn);
    	state.enPassantTarget = Posn{-1, -1};
	}

	// 2. Move/ Capture
	dst = std::move(src);
	src.reset();

	// 3. Promotion
	bool colour = (dst->getColour() == Colour::White);
	if (dst && (dst->getSymbol() == colour ? 'P' : 'p') && m.promo != ' ') {
		switch (m.promo) {
            case 'Q': case 'q': dst = std::make_unique<Queen>(colour); break;
            case 'R': case 'r': dst = std::make_unique<Rook>(colour); break;
            case 'B': case 'b': dst = std::make_unique<Bishop>(colour); break;
            case 'N': case 'n': dst = std::make_unique<Knight>(colour); break;
        }
	}

	// 4. Update enPassantTarget to State
	if ((src->getSymbol() == 'P' || src->getSymbol() == 'p') && abs(end.row - start.row) == 2) {
        int middleRow = (start.row + end.row) / 2;
        state.enPassantTarget = Posn{middleRow, start.col};
    } else {
        state.enPassantTarget = Posn{-1, -1};
    }
	
	// 5. flip turn and update status
	state.turn = (colour ? Colour::Black : Colour::White);
	if (isCheckMate(state.turn)) {
		state.status = ((state.turn == Colour::Black) ? GameStatus::WHITE_WINS : GameStatus::BLACK_WINS);
	} else if (isStaleMate(state.turn)) {
		state.status = GameStatus::STALEMATE;
	} else if (isCheck(state.turn)) {
		state.status = ((state.turn == Colour::Black) ? GameStatus::WHITE_IN_CHECK: GameStatus::BLACK_IN_CHECK);
	} else {
    	state.status = GameStatus::IN_PROGRESS;
	}

	snapshotBoard();
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

			if (!piece || piece->getColour() != enemyColour) continue;
				
			const auto moves = piece->getValidMoves(*this, piecePos);
			if (std::find(moves.begin(), moves.end(), kingPos) != moves.end()) {
				return true;
			}
		}
	}
	return false;

}

bool Board::isCheckMate(Colour c) const {
	// 1. in check state
	// 2. after all legal moves still in check
	if (!isCheck(c)) return false;

	auto moves = legalMoves(c);
	for (const Move &m : moves) {
		Board copy(*this);
		copy.movePiece(m);

		if (!copy.isCheck(c)) return false;
	}
	return true;
}

bool Board::isStaleMate(Colour c) const {
	// 1. not in check state
	// 2. no legal moves
	if (isCheck(c)) return false;
	auto moves = legalMoves(c);
	return moves.empty();
}

void Board::setupDefaultBoard() {
	// Reset board
	for (auto &row : board) {
        for (auto &sq : row) {
            sq.reset();
		}
	}

	// Put Pawn
	for (std::size_t col = 0; col < gridSize; ++col) {
		board[1][col] = std::make_unique<Pawn>(Colour::Black);
		board[6][col] = std::make_unique<Pawn>(Colour::White);
	}

	// Put Other Pieces
	board[0][0] = std::make_unique<Rook>(Colour::Black);
	board[0][7] = std::make_unique<Rook>(Colour::Black);
	board[0][1] = std::make_unique<Knight>(Colour::Black);
	board[0][6] = std::make_unique<Knight>(Colour::Black);
	board[0][2] = std::make_unique<Bishop>(Colour::Black);
	board[0][5] = std::make_unique<Bishop>(Colour::Black);
	board[0][3] = std::make_unique<Queen>(Colour::Black);
	board[0][4] = std::make_unique<King>(Colour::Black);

	board[7][0] = std::make_unique<Rook>(Colour::White);
	board[7][7] = std::make_unique<Rook>(Colour::White);
	board[7][1] = std::make_unique<Knight>(Colour::White);
	board[7][6] = std::make_unique<Knight>(Colour::White);
	board[7][2] = std::make_unique<Bishop>(Colour::White);
	board[7][5] = std::make_unique<Bishop>(Colour::White);
	board[7][3] = std::make_unique<Queen>(Colour::White);
	board[7][4] = std::make_unique<King>(Colour::White);
	 
	state.turn = Colour::White;
    state.status = GameStatus::IN_PROGRESS;
    state.castlingRights = {true, true, true, true};
    state.enPassantTarget = {-1, -1};

	snapshotBoard();
	notifyObservers();
}

State Board::getGameState() const {return state;}


// return the possible move
std::vector<Move> Board::legalMoves(Colour c) const {
	std::vector<Move> legal;
	const bool white = (c == Colour::White);

	for (std::size_t row = 0; row < gridSize; ++row) {
		for (std::size_t col = 0; col < gridSize; ++ col) {
			const auto &piece = board[row][col];
			Posn start{static_cast<int>(row), static_cast<int>(col)};

			if (!piece || piece->getColour() != c) continue;
				
			auto moves = piece->getValidMoves(*this, start);
			if (moves.empty()) continue;

			for (const Posn& end : moves) {

				bool isPawn = (piece->getSymbol() == (white ? 'P' : 'p'));
				bool isPromotion = (isPawn && ((end.row == 0) || (end.col == 7)));

				if (isPromotion) {
					const char promoPieces[4] = {white ? 'Q':'q', white ? 'R':'r', 
							white ? 'B':'b', white ? 'N':'n'};
					for (int i = 0; i < 4; ++i) {
						Move m{start, end, promoPieces[i]};
						Board copy(*this);

						if (!copy.movePiece(m)) continue;
						if (copy.isCheck(c)) continue;

						legal.push_back(m);
					}
				} else {
					Move m{start, end, ' '};

                    Board copy(*this);
                    if (!copy.movePiece(m)) continue;
                    if (copy.isCheck(c)) continue;

                    legal.push_back(m);
				}
            }
		}
	}
	return legal;
}

void Board::changeState(State newState) {
	state = newState;
	notifyObservers();
}


void Board::removePiece(Posn p) {
	std::size_t row = static_cast<std::size_t>(p.row);
    std::size_t col = static_cast<std::size_t>(p.col);

	if (row >= gridSize || col >= gridSize) return;

	board[row][col].reset();

	snapshotBoard();
	notifyObservers();
}

