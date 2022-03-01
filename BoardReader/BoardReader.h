//
// Created by misc1 on 2/26/2022.
//

#ifndef SCRABBLEBOT_BOARDREADER_H
#define SCRABBLEBOT_BOARDREADER_H
#include "../LString/LString.h"

class BoardReader {
public:
    BoardReader();
    BoardReader(LString);
    void buildBoard();
    void printBoard() const;
    void SearchBoardHorizontal();
    string to_string() const;
    LString update_best_word();
    LString get_words_of_row(int);

private:
    int bestX, bestY;
    int curX, curY;
    LString bestWord;
    LString hand;
    bool horizontal;
    vector<LString> board;
    vector<LString> wordsOfRow[15];
};


#endif //SCRABBLEBOT_BOARDREADER_H
