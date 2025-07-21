#ifndef SCOREBOARD_H
#define SCOREBOARD_H



class Scoreboard {
    double whiteScore;
    double blackScore;

public:
    void startGame();
    void endGame();
    void setWhiteScore(double score);
    void setBlackScore(double score);
    void printScore(std::ostream &out) const; 
};

#endif
