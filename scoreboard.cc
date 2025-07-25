#include "scoreboard.h"

void Scoreboard::startGame() {
    whiteScore = 0;
    blackScore = 0;
}

void Scoreboard::endGame() {
    
}

void Scoreboard::addWhiteScore(double score) {
    whiteScore += score;
}

void Scoreboard::addBlackScore(double score) {
    blackScore += score;
}

void Scoreboard::printScore(std::ostream &out) const {
    out << "Final Score:\n";
    out << "White: " << whiteScore << "\n";
    out << "Black: " << blackScore << "\n";
}
