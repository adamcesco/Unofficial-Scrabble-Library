//
// Created by misc1 on 2/26/2022.
//

#ifndef SCRABBLEBOT_BOARDREADER_H
#define SCRABBLEBOT_BOARDREADER_H
#include "../LString/LString.h"

class BoardReader {
public:
    BoardReader();
    BoardReader(const LString&);
    void buildBoard();
    void printBoard() const;
    void SearchBoardHorizontal();   //find all horizontal words that fit into the board
    string to_string() const;       //print the info of the best word for the current board
    LString update_best_word();     //updates the best word for the board so that it is up to date with what was found for the current board
    LString get_words_of_row(int);  //returns a LString that contains all found words that fit into the requested row
    LString row_to_string(int);     //returns a LString that contains all found words that fit into the requested row and each words points
    void check_vertical_compatibility();                        //checks each horizontal word for each row to see if it is compatible with adjacent rows
    vector<LString> return_board_with(const LString&) const;    //this returns a copy of the board that contains the passed LString
    int return_vertical_points(const LString&) const;           //counts all points for all vertical words created by a given horizontal word

private:
    int bestX, bestY;               //holds the coordinate of the first character of the best word for the current board
    LString bestWord;               //holds the best word for the current board
    LString hand;                   //holds the hand of the solver
    bool horizontal;                //states whether the best word for the board is horizontal, this is not supported yet
    vector<LString> board;          //holds the game board
    unordered_set<string> answerSet;
    vector<LString> wordsOfRow[15]; //an array of vectors, where each vector holds the words that fit into the given row
                                    // each row is sorted by myComp as an extension of answers being sorted by myComp
};


#endif //SCRABBLEBOT_BOARDREADER_H
