#include "humanplayer.h"
#include "posn.h"
#include <stdexcept>

Posn HumanPlayer::convertToPosn(const std::string& posnStr) {
    if (posnStr.length() != 2) {
        throw std::invalid_argument("Invalid position string");
    }
    int col = posnStr[0] - 'a';  // a=0, b=1, ..., h=7
    int row = '8' - posnStr[1];  // 1=7, 2=6, ..., 8=0
    if (col < 0 || col > 7 || row < 0 || row > 7) {
        throw std::invalid_argument("Invalid position string");
    }    
    return Posn{row, col};
}

bool HumanPlayer::isValidCommand(const std::string& line) {
    // Check if line starts with "move "
    if (line.substr(0, 5) != "move ") {
        return false;
    }
    
    // Minimum length check: "move e2 e4" = 10 chars
    if (line.length() < 10) {
        return false;
    }
    
    try {
        // Try to parse the from and to positions
        std::string fromStr = line.substr(5, 2);
        std::string toStr = line.substr(8, 2);
        
        convertToPosn(fromStr);
        convertToPosn(toStr);
        
        // Check for promotion case: "move e7 e8 Q"
        if (line.length() >= 12 && line[10] == ' ') {
            char promo = line[11];
            // Valid promotion pieces
            if (promo != 'Q' && promo != 'R' && promo != 'B' && promo != 'N' &&
                promo != 'q' && promo != 'r' && promo != 'b' && promo != 'n') {
                return false;
            }
        } else if (line.length() != 10) {
            // If not promotion, should be exactly 10 characters
            return false;
        }
        
        return true;
    } catch (const std::invalid_argument&) {
        return false;
    }
}

Move HumanPlayer::makeMove(Board &board, const std::string& line) {
    // Assume line is already validated by isValidCommand
    Posn from = convertToPosn(line.substr(5, 2));
    Posn to = convertToPosn(line.substr(8, 2));
    
    if (line.length() >= 12 && line[10] == ' ') {
        // Pawn promotion case
        char promo = line[11];
        return Move{from, to, promo};
    } else {
        // Normal move
        return Move{from, to, ' '};
    }
}
