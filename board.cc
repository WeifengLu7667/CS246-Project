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

	std::size_t n = gridSize;
    for (std::size_t r = 0; r < n; ++r) {

        for (std::size_t c = 0; c < n; ++c) {
            const auto &piece = board[r][c];
            if (piece) {state.board[r][c] = piece->getSymbol();}
            else state.board[r][c] = ((r + c) & 1) ? '_' : ' ';
        }
    }
}


bool Board::canCastle(Colour c, bool kingSide) const {
	const int row = (c == Colour::White ? 7 : 0);
	const int kCol = 4;
	const int rCol = kingSide ? 7 : 0;
    const int dir = kingSide ? 1 : -1;

	// 1. still have the right
    const CastlingInfo &cr = state.castlingRights;
    if (c == Colour::White && (kingSide ? !cr.whiteKingSide : !cr.whiteQueenSide)) return false;
    if (c == Colour::Black && (kingSide ? !cr.blackKingSide : !cr.blackQueenSide)) return false;

	// 2. check existence of root and king
	if (!board[row][kCol] || !board[row][rCol]) return false;
	if (board[row][kCol]->getSymbol() != (c == Colour::White ? 'K' : 'k')) return false;
	if (board[row][rCol]->getSymbol() != (c == Colour::White ? 'R' : 'r')) return false;

	// 3. check the cells in between are empty
	for (int c = kCol + dir; c != rCol; c += dir) {
        if (board[row][c]) return false;
	}
	// 4. king is not currently in check and will not cross/land in check
    if (isCheck(c)) return false;
    Posn step { row, kCol + dir };
    Posn dest { row, kCol + 2*dir };
    Colour enemy = (c == Colour::White ? Colour::Black : Colour::White);
    if (squareIsAttacked(step, enemy) || squareIsAttacked(dest, enemy)) return false;
    return true;
}

bool Board::squareIsAttacked(Posn p, Colour colour) const {
	for (std::size_t r = 0; r < gridSize; ++r)
        for (std::size_t c = 0; c < gridSize; ++c) {
            if (!board[r][c] || board[r][c]->getColour() != colour) continue;
			auto moves = board[r][c]->getValidMoves(*this, {int(r),int(c)});
            if (std::find(moves.begin(), moves.end(), p) != moves.end()) return true;
        }
    return false;
}

bool Board::movePieceInternal(const Move &m) {
	// 0. Sanity Check
	Posn start = m.from;
	Posn end = m.to;

	if (start.row < 0 || start.row >= static_cast<int>(gridSize) ||
		start.col < 0 || start.col >= static_cast<int>(gridSize) ||
		end.row < 0 || end.row >= static_cast<int>(gridSize) ||
		end.col < 0 || end.col >= static_cast<int>(gridSize)) return false;
	auto &src = board[start.row][start.col];
	auto &dst = board[end.row][end.col];

	if (!src) return false;

	/* 1‑A  king moves → both rights for that colour are lost */
	if (King* k = dynamic_cast<King*>(src.get())) {
		if (k->getColour() == Colour::White) {
			state.castlingRights.whiteKingSide  = false;
			state.castlingRights.whiteQueenSide = false;
		} else {
			state.castlingRights.blackKingSide  = false;
			state.castlingRights.blackQueenSide = false;
		}
	}

	// 1‑B  rook moves → the corresponding side is lost             
	if (Rook* r = dynamic_cast<Rook*>(src.get())) {
		if (r->getColour() == Colour::White) {
			if (start == Posn{7,0}) state.castlingRights.whiteQueenSide = false;
			if (start == Posn{7,7}) state.castlingRights.whiteKingSide  = false;
		} else {
			if (start == Posn{0,0}) state.castlingRights.blackQueenSide = false;
			if (start == Posn{0,7}) state.castlingRights.blackKingSide  = false;
		}
	}
	
	// Clear en passant target at the start of every move
	Posn previousEnPassantTarget = state.enPassantTarget;
	state.enPassantTarget = Posn{-1, -1};

	// 1.1 En Passant Capture BEFORE we move the pawn
	if ((src->getSymbol() == 'P' || src->getSymbol() == 'p') && end == previousEnPassantTarget) {
    	int capturedRow = (src->getColour() == Colour::White) ? end.row + 1 : end.row - 1;
    	Posn capturedPosn = Posn{capturedRow, end.col};
    	removePiece(capturedPosn);
	}

	// 1.2 Castling 
	// Check if this is a castling move
	if ((src->getSymbol() == 'K' || src->getSymbol() == 'k') &&
		start.row == end.row && std::abs(static_cast<int>(end.col) - static_cast<int>(start.col)) == 2) {
		
		int row = (src->getColour() == Colour::White) ? 7 : 0;

		// King-side castling (short): king moves from e to g (col 4 to 6)
		if (end.col == 6) {
			board[row][5] = std::move(board[row][7]);
		}
		// Queen-side castling (long): king moves from e to c (col 4 to 2)
		else if (end.col == 2) {
			board[row][3] = std::move(board[row][0]);
		}
	}


	// 2. Move/ Capture
	dst = std::move(src);
	src.reset();

	// 3. Promotion
	bool isWhite = (dst->getColour() == Colour::White);
	Colour c = dst->getColour();


	if (dst && (dst->getSymbol() == (isWhite ? 'P' : 'p')) && m.promo != ' ') {
		switch (std::tolower(m.promo)) {
        	case 'q': dst = std::make_unique<Queen>(c); break;
        	case 'r': dst = std::make_unique<Rook>(c); break;
        	case 'b': dst = std::make_unique<Bishop>(c); break;
        	case 'n': dst = std::make_unique<Knight>(c); break;
        	default : break;
    	}
	}


	// 4. Update enPassantTarget to State (only for double pawn moves)
	if ((dst && (dst->getSymbol() == 'P' || dst->getSymbol() == 'p')) && std::abs(end.row - start.row) == 2) {
    	int middleRow = (start.row + end.row) / 2;
    	state.enPassantTarget = Posn{middleRow, end.col};
	}
	
	// 5. Just flip the turn - no status checking
	state.turn = (isWhite ? Colour::Black : Colour::White);

	return true;
}

bool Board::movePiece(const Move &m) {
	// 0. Sanity Check
	Posn start = m.from;
	Posn end = m.to;
	if (start.row < 0 || start.row >= static_cast<int>(gridSize) ||
		start.col < 0 || start.col >= static_cast<int>(gridSize) ||
		end.row < 0 || end.row >= static_cast<int>(gridSize) ||
		end.col < 0 || end.col >= static_cast<int>(gridSize)) return false;

	auto &src = board[start.row][start.col];

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

	// Use internal function to perform the actual move
	if (!movePieceInternal(m)) return false;

	// 3. Update game status after the move
	if (isCheckMate(state.turn)) {
		state.status = ((state.turn == Colour::Black) ? GameStatus::WHITE_WINS : GameStatus::BLACK_WINS);
	} else if (isStaleMate(state.turn)) {
		state.status = GameStatus::STALEMATE;
	} else if (isCheck(state.turn)) {
		state.status = ((state.turn == Colour::Black) ? GameStatus::WHITE_IN_CHECK: GameStatus::BLACK_IN_CHECK);
	} else {
    	state.status = GameStatus::IN_PROGRESS;
	}

	// 4. Update observers
	snapshotBoard();
	notifyObservers();

	return true;
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
		copy.movePieceInternal(m); // Pass false to prevent infinite recursion
		if (!copy.isCheck(c)) {return false;}
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

			// add more possible end posn for king castling
			bool isKing = (piece->getSymbol() == (white ? 'K' : 'k'));
			if (isKing) {
				// Use the correct castling row (7 for white, 0 for black)
				int castlingRow = (c == Colour::White ? 7 : 0);
				// king side
				if (canCastle(c, true))
					legal.push_back(Move{ start, Posn{castlingRow, 6}, ' ' });
				// queen side
				if (canCastle(c, false))
					legal.push_back(Move{ start, Posn{castlingRow, 2}, ' ' });
			}

			if (moves.empty()) continue;

			// generate legal move for promotion
			for (const Posn& end : moves) {

				bool isPawn = (piece->getSymbol() == (white ? 'P' : 'p'));
				bool isPromotion = (isPawn && ((end.row == 0) || (end.row == 7)));

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
	
	// Clear existing pieces
	for (auto &row : board) {
		for (auto &sq : row) {
			sq.reset();
		}
	}
	
	// Create piece objects from the character representation
	for (std::size_t row = 0; row < gridSize; ++row) {
		for (std::size_t col = 0; col < gridSize; ++col) {
			char pieceChar = state.board[row][col];
			if (pieceChar != ' ' && pieceChar != '_') {
				Colour c = (std::isupper(pieceChar)) ? Colour::White : Colour::Black;
				
				switch (std::tolower(pieceChar)) {
					case 'p': board[row][col] = std::make_unique<Pawn>(c); break;
					case 'r': board[row][col] = std::make_unique<Rook>(c); break;
					case 'n': board[row][col] = std::make_unique<Knight>(c); break;
					case 'b': board[row][col] = std::make_unique<Bishop>(c); break;
					case 'q': board[row][col] = std::make_unique<Queen>(c); break;
					case 'k': board[row][col] = std::make_unique<King>(c); break;
					default: break; // Invalid piece character, leave empty
				}
			}
		}
	}
	
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

