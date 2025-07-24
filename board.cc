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
#include <memory>
#include <cstddef> // try to remove later
#include <cctype> // try to remove later
#include <cmath> // try to remove later
#include <utility>

// Big 5
Board::Board(std::size_t gridSize): board(gridSize), gridSize{gridSize} {
	// TextDisplay will be attached by the Game controller
	for (auto& row : board) row.resize(gridSize);
	state.board.resize(gridSize, std::vector<char>(gridSize, ' '));
}

Board::Board(const Board& other): board(other.gridSize), 
			gridSize{other.gridSize}, state{other.state} {
    for (auto& row : board) row.resize(gridSize);

    for (std::size_t r = 0; r < gridSize; ++r) {
        for (std::size_t c = 0; c < gridSize; ++c) {
            if (other.board[r][c]) {
                board[r][c] = other.board[r][c]->clone();
			}
		}
	}
	state.board.resize(gridSize, std::vector<char>(gridSize, ' '));
}



void Board::snapshotBoard() {

	cout << "debug3.5.10.1" << endl;
	std::size_t n = gridSize;
    for (std::size_t r = 0; r < n; ++r) {

		cout << "debug3.5.10.2" << endl;
        for (std::size_t c = 0; c < n; ++c) {
            const auto &piece = board[r][c];
			cout << "debug3.5.9.3" << endl;
            if (piece) {state.board[r][c] = piece->getSymbol(); cout << "debug3.5.9.4" << endl;}
            else state.board[r][c] = ((r + c) & 1) ? '_' : ' ';
        }
    }
}

bool Board::movePieceInternal(const Move &m) {
	// 0. Sanity Check
	Posn start = m.from;
	Posn end = m.to;

	if (start.row < 0 || start.row >= static_cast<int>(gridSize) ||
		start.col < 0 || start.col >= static_cast<int>(gridSize) ||
		end.row < 0 || end.row >= static_cast<int>(gridSize) ||
		end.col < 0 || end.col >= static_cast<int>(gridSize)) return false;
	cout << "debug3.5.1" << endl;
	auto &src = board[start.row][start.col];
	auto &dst = board[end.row][end.col];

	cout << "debug3.5.2" << endl;
	if (!src) return false;

	cout << "debug3.5.3" << endl;

	// Clear en passant target at the start of every move
	Posn previousEnPassantTarget = state.enPassantTarget;
	state.enPassantTarget = Posn{-1, -1};

	cout << "debug3.5.4" << endl;
	// 1. En Passant Capture BEFORE we move the pawn
	if ((src->getSymbol() == 'P' || src->getSymbol() == 'p') && end == previousEnPassantTarget) {
    	int capturedRow = (src->getColour() == Colour::White) ? end.row + 1 : end.row - 1;
    	Posn capturedPosn = Posn{capturedRow, end.col};
    	removePiece(capturedPosn);
	}

	cout << "debug3.5.5" << endl;
	// 2. Move/ Capture
	dst = std::move(src);
	src.reset();

	cout << "debug3.5.6" << endl;
	// 3. Promotion
	bool isWhite = (dst->getColour() == Colour::White);
	Colour c = dst->getColour();


	cout << "debug3.5.7" << endl;
	if (dst && (dst->getSymbol() == (isWhite ? 'P' : 'p')) && m.promo != ' ') {
		switch (std::tolower(m.promo)) {
        	case 'q': dst = std::make_unique<Queen>(c); break;
        	case 'r': dst = std::make_unique<Rook>(c); break;
        	case 'b': dst = std::make_unique<Bishop>(c); break;
        	case 'n': dst = std::make_unique<Knight>(c); break;
        	default : break;
    	}
	}


	cout << "debug3.5.8" << endl;
	// 4. Update enPassantTarget to State (only for double pawn moves)
	if ((dst && (dst->getSymbol() == 'P' || dst->getSymbol() == 'p')) && std::abs(end.row - start.row) == 2) {
    	int middleRow = (start.row + end.row) / 2;
    	state.enPassantTarget = Posn{middleRow, end.col};
	}
	

	cout << "debug3.5.9" << endl;
	// 5. flip turn and update status
	state.turn = (isWhite ? Colour::Black : Colour::White);
	cout << "debug3.5.9.1" << endl;
	if (isCheckMate(state.turn)) {
		cout << "debug3.5.9.2" << endl;
		state.status = ((state.turn == Colour::Black) ? GameStatus::WHITE_WINS : GameStatus::BLACK_WINS);
	} else if (isStaleMate(state.turn)) {
		cout << "debug3.5.9.3" << endl;
		state.status = GameStatus::STALEMATE;
	} else if (isCheck(state.turn)) {
		cout << "debug3.5.9.4" << endl;
		state.status = ((state.turn == Colour::Black) ? GameStatus::WHITE_IN_CHECK: GameStatus::BLACK_IN_CHECK);
	} else {
		cout << "debug3.5.9.5" << endl;
    	state.status = GameStatus::IN_PROGRESS;
	}

	cout << "debug3.5.10" << endl;
	snapshotBoard();
	cout << "here?" << endl;
	notifyObservers();

	cout << "debug3.5.11" << endl;
	return true;
}

bool Board::movePiece(const Move &m) {
	// 0. Sanity Check
	Posn start = m.from;
	Posn end = m.to;
	cout << "debug3.1" << endl;
	if (start.row < 0 || start.row >= static_cast<int>(gridSize) ||
		start.col < 0 || start.col >= static_cast<int>(gridSize) ||
		end.row < 0 || end.row >= static_cast<int>(gridSize) ||
		end.col < 0 || end.col >= static_cast<int>(gridSize)) return false;

	auto &src = board[start.row][start.col];

	cout << "debug3.2" << endl;
	if (!src) return false;
	Colour c = src->getColour();

	// 1. Legal Move? 
	std::vector<Move> all = legalMoves(c);
	bool legal = false;

	cout << "debug3.3" << endl;
	for (const Move &lm : all) {
		if (lm.from == m.from && lm.to == m.to && lm.promo == m.promo) {
            legal = true;
            break;
		}
	}

	cout << "debug3.4" << endl;
	if (!legal) return false;

	// Use internal function to perform the actual move
	cout << "debug3.5" << endl;
	return movePieceInternal(m);
}

Piece* Board::getPieceAt(std::size_t x, std::size_t y) const {
	if (x < 0 || x >= gridSize || y < 0 || y >= gridSize) {
        return nullptr;
    }
	return board[x][y].get();
}

bool Board::isCheck(Colour c) const {
	char kingSymbol = (c == Colour::White) ? 'K' : 'k';
	Posn kingPos = {-1, -1};
	Posn piecePos = {-1, -1};
	cout << "debug3.5.9.5.1.1" << endl;
	for (std::size_t row = 0; row < gridSize; ++row) {
		cout << "debug3.5.9.5.1.2" << endl;
		for (std::size_t col = 0; col < gridSize; ++ col) {
			cout << "debug3.5.9.5.1.3" << endl;
			if (board[row][col] && board[row][col]->getSymbol() == kingSymbol) {
				cout << "debug3.5.9.5.1.4" << endl;
				kingPos = {static_cast<int>(row), static_cast<int>(col)};
				break;
			}
		}
	}
	cout << "debug3.5.9.5.1.5" << endl;
	Colour enemyColour = (c == Colour::White) ? Colour::Black : Colour::White;
	cout << "debug3.5.9.5.1.6" << endl;
	for (std::size_t row = 0; row < gridSize; ++row) {
		cout << "debug3.5.9.5.1.7" << endl;
		for (std::size_t col = 0; col < gridSize; ++ col) {
			cout << "debug3.5.9.5.1.8" << endl;
			const auto &piece = board[row][col];
			piecePos = {static_cast<int>(row), static_cast<int>(col)};
			cout << "debug3.5.9.5.1.9" << endl;
			if (!piece || piece->getColour() != enemyColour) continue;
			cout << "debug3.5.9.5.1.10" << endl;	
			const auto moves = piece->getValidMoves(*this, piecePos);
			cout << "debug3.5.9.5.1.11" << endl;
			if (std::find(moves.begin(), moves.end(), kingPos) != moves.end()) {
				cout << "debug3.5.9.5.1.12" << endl;
				return true;
			}
		}
		cout << "debug3.5.9.5.1.13" << endl;
	}
	cout << "debug3.5.9.5.1.14" << endl;
	return false;

}

bool Board::isCheckMate(Colour c) const {
	cout << "debug3.5.9.5.1" << endl;
	// 1. in check state
	// 2. after all legal moves still in check
	if (!isCheck(c)) return false;
	cout << "debug3.5.9.5.2" << endl;
	auto moves = legalMoves(c);
	cout << "debug3.5.9.5.3" << endl;
	for (const Move &m : moves) {
		cout << "debug3.5.9.5.4" << endl;
		Board copy(*this);
		copy.movePieceInternal(m);
		cout << "debug3.5.9.5.5" << endl;
		if (!copy.isCheck(c)) {cout << "debug3.5.9.5.6" << endl; return false;}
	}
	cout << "debug3.5.9.5.7" << endl;
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

						if (!copy.movePieceInternal(m)) continue;
						if (copy.isCheck(c)) continue;

						legal.push_back(m);
					}
				} else {
					Move m{start, end, ' '};

                    Board copy(*this);
                    if (!copy.movePieceInternal(m)) continue;
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

