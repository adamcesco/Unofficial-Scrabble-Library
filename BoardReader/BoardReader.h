//
// Created by misc1 on 2/26/2022.
//

#ifndef SCRABBLEBOT_BOARDREADER_H
#define SCRABBLEBOT_BOARDREADER_H
#include "../HandManager/HandManager.h"

class BoardReader {
public:
    BoardReader();
    void buildBoard();
    void printBoard() const;
    void traverseBoard();

private:
    int bestX, bestY;
    int curX, curY;
    string bestWord;
    char board[15][15];
};


#endif //SCRABBLEBOT_BOARDREADER_H
