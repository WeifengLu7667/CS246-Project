#include "game.h"
#include <iostream>
#include <string>
#include "colour.h"
#include <vector>
#include "board.h"
#include "humanplayer.h"
#include "computerplayer.h"
#include "player.h"
#include "strategy.h"
#include "level1.h"
#include "level2.h"
#include "level3.h"

using namespace std;

Posn Game::convertToPosn(const std::string& posnStr) {// may throw invalid_argument if the position string is invalid
    if (posnStr.length() != 2) {
        throw std::invalid_argument("Invalid position string");
    }
    // convert the position string to a Posn object
    // a1 = row 7, col 0; h8 = row 0, col 7
    int col = posnStr[0] - 'a';  // a=0, b=1, ..., h=7
    int row = '8' - posnStr[1];  // 1=7, 2=6, ..., 8=0
    if (col < 0 || col > 7 || row < 0 || row > 7) {
        throw std::invalid_argument("Invalid position string");
    }    
    return Posn{row, col};
}

void Game::displayBoard(const std::vector<std::vector<char>>& setupBoard) {
    cout << "Current board:" << endl;
    for (int i = 0; i < 8; i++) {
        cout << 8 - i << " ";
        for (int j = 0; j < 8; j++) {
            cout << setupBoard[i][j];
        }
        cout << endl;
    }
    cout << endl << "  abcdefgh" << endl << endl;
}

void Game::gameRun() { 
    cout << "Hello World!" << endl; // to be removed
    cout << "Test" << endl; // to be removed
    string line;
    useDefaultBoard = true;
    isRunning = false;
    scoreboard.startGame();

    while (getline(cin, line)) {
        if (line == "resign" && isRunning) {
            //add code here
            State currentState = board.getGameState();
            if (currentState.turn == Colour::White) {
                cout << "black player wins" << endl;
                scoreboard.addBlackScore(1);
            } else {
                cout << "white player wins" << endl;
                scoreboard.addWhiteScore(1);
            }
            isRunning = false;
            useDefaultBoard = true;

        } else if (line == "resign" && !isRunning) {
            cout << "can't resign, because game is not running" << endl;
        } else if (line == "setup" && !isRunning) {
            // setup mode
            Colour turn = Colour::White;
            bool oneWhiteKing = false;
            bool oneBlackKing = false;
            CastlingInfo castlingRights{false, false, false, false};
            Posn enPassantTarget{-1, -1};
            vector<vector<char>> setupBoard(8, vector<char>(8));
            // initialize the empty board
            for (int i = 0; i < 8; i++) {
                for (int j = 0; j < 8; j++) {
                    if ((i + j) % 2 == 0) {
                        setupBoard[i][j] = ' ';
                    } else {
                        setupBoard[i][j] = '_'; // black space
                    }
                }
            }
            displayBoard(setupBoard);
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
                                useDefaultBoard = false;
                                break;
                            }    
                        }
                    } else {
                        cout << "invalid board, can't exit. King number is not correct" << endl;
                    }
                } else if (line == "= black") {
                    turn = Colour::Black;
                } else if (line == "= white") {
                    turn = Colour::White;
                } else if (line[0] == '-') {
                    try {
                        Posn pieceToRemove = convertToPosn(line.substr(2, 4));
                        if (setupBoard[pieceToRemove.row][pieceToRemove.col] != '_' && setupBoard[pieceToRemove.row][pieceToRemove.col] != ' ') {
                            if ((pieceToRemove.row + pieceToRemove.col) % 2 == 0) {
                                setupBoard[pieceToRemove.row][pieceToRemove.col] = ' ';
                            } else {
                                setupBoard[pieceToRemove.row][pieceToRemove.col] = '_'; // black space
                            }
                            // display the board
                            displayBoard(setupBoard);
                        } else {
                            cout << "the position is already empty" << endl;
                        }
                    } catch (const std::invalid_argument&) {
                        cout << "invalid command in setup mode" << endl;
                    }
                } else if (line[0] == '+') {
                    try {
                        Posn pieceToAdd = convertToPosn(line.substr(4, 6));
                        char pieceSymbol = line[2];
                        if (setupBoard[pieceToAdd.row][pieceToAdd.col] == '_' || setupBoard[pieceToAdd.row][pieceToAdd.col] == ' ') {
                            setupBoard[pieceToAdd.row][pieceToAdd.col] = pieceSymbol;
                            // display the board
                            displayBoard(setupBoard);
                        } else {
                            cout << "the position is already occupied" << endl;
                        }
                    } catch (const std::invalid_argument&) {
                        cout << "invalid command in setup mode" << endl;
                    }
                } else {
                    cout << "invalid command in setup mode" << endl;
                }
            }
            //the setup mode is done, we can start the game
        } else if (line == "setup" && !isRunning) {
            cout << "can't enter setup mode, because game is running" << endl;
        } else if (line.substr(0, 4) == "move" && isRunning) {
            // the code in this else if block needs to be changed -----------------------------------------------------------
            try {
                Posn from = convertToPosn(line.substr(5, 7));
                Posn to = convertToPosn(line.substr(8, 10));
                if (line.length() == 12) {
                    char promo = line[11];
                    Move move{from, to, promo};
                    board.movePiece(move);
                } else {
                    Move move{from, to, ' '};
                    board.movePiece(move);
                }
            } catch (const std::invalid_argument&) {
                cout << "invalid move command" << endl;
            }
        } else if (line.substr(0, 4) == "move" && !isRunning) {
            cout << "can't move, because game is not running" << endl;
        } else if (line.substr(0, 4) == "undo" && isRunning) {
            // add code here
        } else if (line.substr(0, 4) == "undo" && !isRunning) {
            cout << "can't undo, because game is not running" << endl;
        } else if (line.substr(0, 4) == "game" && isRunning) {
            cout << "can't start a new game, because currentgame is running" << endl;
        } else if (line.substr(0, 4) == "game" && !isRunning) {
            size_t spacePos = line.rfind(' ');
            string whitePlayerType;
            string blackPlayerType;
            bool validPlayers = true;

            // read the player types
            if (spacePos == string::npos) {
                cout << "invalid game command" << endl;
            } else {
                whitePlayerType = line.substr(5, spacePos - 5);
                blackPlayerType = line.substr(spacePos + 1);
            }

            // initialize white player
            if(whitePlayerType == "human") {
                whitePlayer = make_unique<HumanPlayer>();
            } else if (whitePlayerType == "computer[1]") {
                unique_ptr<Strategy> strategy = make_unique<Level1>();
                whitePlayer = make_unique<ComputerPlayer>(move(strategy));
            } else if (whitePlayerType == "computer[2]") {
                unique_ptr<Strategy> strategy = make_unique<Level2>();
                whitePlayer = make_unique<ComputerPlayer>(move(strategy));
            } else if (whitePlayerType == "computer[3]") {
                unique_ptr<Strategy> strategy = make_unique<Level3>();
                whitePlayer = make_unique<ComputerPlayer>(move(strategy));
            } else if (whitePlayerType == "computer[4]") {
                // add code here for level 4
                cout << "level 4 is not supported yet" << endl;
                validPlayers = false;
            } else {
                cout << "invalid player type" << endl;
                validPlayers = false;
            }

            // initialize black player
            if (blackPlayerType == "human") {
                blackPlayer = make_unique<HumanPlayer>();
            } else if (blackPlayerType == "computer[1]") {
                unique_ptr<Strategy> strategy = make_unique<Level1>();
                blackPlayer = make_unique<ComputerPlayer>(move(strategy));
            } else if (blackPlayerType == "computer[2]") {
                unique_ptr<Strategy> strategy = make_unique<Level2>();
                blackPlayer = make_unique<ComputerPlayer>(move(strategy));
            } else if (blackPlayerType == "computer[3]") {
                unique_ptr<Strategy> strategy = make_unique<Level3>();
                blackPlayer = make_unique<ComputerPlayer>(move(strategy));
            } else if (blackPlayerType == "computer[4]") {
                // add code here for level 4
                cout << "level 4 is not supported yet" << endl;
                validPlayers = false;
            } else {
                cout << "invalid player type" << endl;
                validPlayers = false;
            }
            if (validPlayers) {
                // start the game
                if (useDefaultBoard) {// if the user didn't use setup mode, we set up the default board
                    board.setupDefaultBoard();
                }
                isRunning = true;
            }

        } else if (line.substr(0, 4) == "help") {
            cout << "this feature is not currently supported" << endl;
            // add code for bonus feature
        } else {
            cout << "invalid command" << endl;
        } 



    }
    cout << "error reading input" << endl;
}
