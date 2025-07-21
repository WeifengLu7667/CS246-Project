#ifndef SCOREBOARD_H
#define SCOREBOARD_H

#include <iostream>

class Scoreboard {
    double whiteScore; // 0.5 means a draw
    double blackScore;

public:
    void startGame();
    void endGame();
    void addWhiteScore(double score); 
    void addBlackScore(double score);
    void printScore(std::ostream &out) const; 
};

#endif
