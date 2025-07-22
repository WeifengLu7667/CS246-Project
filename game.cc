#include "game.h"
#include <iostream>
#include <string>
#include "colour.h"
#include <vector>
#include "board.h"
using namespace std;

void Game::gameRun() {
    cout << "Hello World!" << endl;
    cout << "Test" << endl;
    string line;

    while (getline(cin, line)) {
        if (line == "resign") {
            //add code here
        } else if (line == "setup") {
            Colour turn;
            bool oneWhiteKing = false;
            bool oneBlackKing = false;
            CastlingInfo castlingRights{false, false, false, false};
            Posn enPassantTarget{-1, -1};
            vector<vector<char>> setupBoard(8, vector<char>(8));
            while (getline(cin, line)) {
                
                if (line == "done") { // if requirements are met, we can exit setup mode and start the game
                    // we assume castling and en passant are invalid

                    if (oneWhiteKing && oneBlackKing) {// first check if both kings are present
                        Board tempBoard;
                        State state{turn, GameStatus::IN_PROGRESS, castlingRights, enPassantTarget, setupBoard};
                        tempBoard.changeState(state);
                        if (tempBoard.isCheck(Colour::White) || tempBoard.isCheck(Colour::Black)) {
                            cout << "invalid board, can't exit. at least one king is in check" << endl;
                        } else {
                            bool valid = true;
                            for (int i = 0; i < 8; i++) {
                                if (setupBoard[0][i] == 'p' || setupBoard[0][i] == 'P' || setupBoard[7][i] == 'p' || setupBoard[7][i] == 'P') {
                                    cout << "invalid board, can't exit. pawns are not allowed in the first or last row" << endl;
                                    valid = false;
                                    break;
                                }
                            }
                            if (valid) {
                                // all requirements are met, we can exit setup mode and start the game
                                board.changeState(state);
                            }    
                        }
                    } else {
                        cout << "invalid board, can't exit. King number is not correct" << endl;
                    }


                } else if (line == "= black") {
                    turn = Colour::Black;
                } else if (line == "= white") {
                    turn = Colour::White;
                } else if (line.substr(0, 1) == "-") {
                    //add code here
                } else if (line.substr(0, 1) == "+") {
                    //add code here
                }

            }
        }


    }
}