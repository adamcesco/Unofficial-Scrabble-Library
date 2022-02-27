//
// Created by misc1 on 2/26/2022.
//

#ifndef SCRABBLEBOT_BOARDREADER_H
#define SCRABBLEBOT_BOARDREADER_H
#include "../HandManager/HandManager.h"

class BoardReader {
public:
    BoardReader();
    BoardReader(string);
    void buildBoard();
    void printBoard() const;
    void SearchBoardHorizontal();
    string to_string() const;

private:
    int bestX, bestY;
    int curX, curY;
    string bestWord;
    string hand;
    Letter board[15][15];
    bool horizontal;
};


#endif //SCRABBLEBOT_BOARDREADER_H
