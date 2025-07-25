#include "level5.h"
#include "move.h"
#include "board.h"
#include <random>
#include <ctime>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <iostream>
#include <limits>

using namespace std;

Level5::Level5() {
    // No member variables to initialize
}

// Piece value evaluation
int getPieceValueLevel5(char symbol) {
    switch (symbol) {
        case 'Q': case 'q': return 9;
        case 'R': case 'r': return 5;
        case 'B': case 'b': return 3;
        case 'N': case 'n': return 3;
        case 'P': case 'p': return 1;
        case 'K': case 'k': return 1000;
        default: return 0;
    }
}

// Evaluate board position from the perspective of the given color
int evaluateBoard(const Board& board, Colour colour) {
    int score = 0;
    Colour opponent = (colour == Colour::White) ? Colour::Black : Colour::White;
    
    // Check for terminal positions
    if (board.isCheckMate(colour)) {
        return -10000;  // We lose
    }
    if (board.isCheckMate(opponent)) {
        return 10000;   // We win
    }
    if (board.isStaleMate(colour) || board.isStaleMate(opponent)) {
        return 0;       // Draw
    }
    
    // Material evaluation
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            Piece* piece = board.getPieceAt(row, col);
            if (piece) {
                int value = getPieceValueLevel5(piece->getSymbol());
                if (piece->getColour() == colour) {
                    score += value;
                } else {
                    score -= value;
                }
            }
        }
    }
    
    // Add positional bonus for check
    if (board.isCheck(opponent)) {
        score += 50;
    }
    if (board.isCheck(colour)) {
        score -= 50;
    }
    
    return score;
}

// Minimax algorithm with alpha-beta pruning
int minimax(Board board, int depth, bool isMaximizing, Colour colour, int alpha, int beta) {
    if (depth == 0) {
        return evaluateBoard(board, colour);
    }
    
    Colour currentPlayer = isMaximizing ? colour : 
                          (colour == Colour::White ? Colour::Black : Colour::White);
    
    vector<Move> moves = board.legalMoves(currentPlayer);
    
    if (moves.empty()) {
        return evaluateBoard(board, colour);
    }
    
    if (isMaximizing) {
        int maxEval = numeric_limits<int>::min();
        for (const Move& move : moves) {
            Board newBoard = board;  // Copy constructor
            if (newBoard.movePiece(move)) {
                int eval = minimax(newBoard, depth - 1, false, colour, alpha, beta);
                maxEval = max(maxEval, eval);
                alpha = max(alpha, eval);
                if (beta <= alpha) {
                    break;  // Alpha-beta pruning
                }
            }
        }
        return maxEval;
    } else {
        int minEval = numeric_limits<int>::max();
        for (const Move& move : moves) {
            Board newBoard = board;  // Copy constructor
            if (newBoard.movePiece(move)) {
                int eval = minimax(newBoard, depth - 1, true, colour, alpha, beta);
                minEval = min(minEval, eval);
                beta = min(beta, eval);
                if (beta <= alpha) {
                    break;  // Alpha-beta pruning
                }
            }
        }
        return minEval;
    }
}

Move Level5::chooseMove(Board &board, Colour colour) {
    const int SEARCH_DEPTH = 2;  // Constant instead of member variable
    
    std::vector<Move> allMoves = board.legalMoves(colour);
    
    if (allMoves.empty()) {
        // Should not happen in legal game state, but return a dummy move
        return Move{{0, 0}, {0, 0}, ' '};
    }
    
    // Initialize random generator (same pattern as Level4)
    static bool seeded = false;
    if (!seeded) {
        std::srand(std::time(nullptr));
        seeded = true;
    }
    
    std::vector<Move> bestMoves;
    int bestScore = numeric_limits<int>::min();
    
    for (const Move& move : allMoves) {
        Board newBoard = board;  // Copy constructor
        if (newBoard.movePiece(move)) {
            int score = minimax(newBoard, SEARCH_DEPTH - 1, false, colour, 
                              numeric_limits<int>::min(), numeric_limits<int>::max());
            
            if (score > bestScore) {
                bestScore = score;
                bestMoves.clear();  // Clear previous best moves
                bestMoves.push_back(move);
            } else if (score == bestScore) {
                bestMoves.push_back(move);  // Add to list of equally good moves
            }
        }
    }
    
    // Randomly select from the best moves
    return bestMoves[std::rand() % bestMoves.size()];
}





