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
            vector<vector<char>> board(8, vector<char>(8));
            while (getline(cin, line)) {
                
                if (line == "done") {
                    CastlingInfo castlingInfo{false, false, false, false};
                    Posn enPassantTarget;
                    //add code here
                    if (oneWhiteKing && oneBlackKing) {// first check if both kings are present
                        Board tempBoard;
                        State state{turn, State::Status::IN_PROGRESS, };
                        tempBoard.changeState()

                    break;
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