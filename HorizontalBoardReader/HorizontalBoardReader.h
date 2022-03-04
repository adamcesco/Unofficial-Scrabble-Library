//
// Created by misc1 on 2/26/2022.
//

#ifndef SCRABBLEBOT_BOARDREADER_H
#define SCRABBLEBOT_BOARDREADER_H
#include "../LString/LString.h"

class HorizontalBoardReader {
public:
    HorizontalBoardReader();
    explicit HorizontalBoardReader(const LString&);
    void build_board();
    void print_board() const;
    void search_board_for_words();  //find all horizontal words that fit into the board
    string to_string() const;       //print the info of the best word for the current board
    LString update_best_hor_word();

    void check_hor_words_perpendicular();
    LString get_best_hor_word() const{return bestHWord;}
    int get_best_hor_x() const{return bestHX;}
    int get_best_hor_y() const{return bestHY;}
    Type get_reader_type() const{return HORIZONTAL;}
    int points_of_best_hor_word(){return perpendicular_points(bestHWord) + bestHWord.get_letter_points();}
    void reset_all_data();
    void set_hand(const LString& passed){hand = passed;}
    void place_into_board(const LString&);    //place a word (vertical or horizontal) onto the board, only the first Letter of the word needs to have its coordinates defined
    void filter_scrabble_words_by_hand();
    vector<LString> return_words_of_row(int);     //needs testing
    vector<LString> get_board(){return board;}

protected:
    vector<LString> return_board_with(const LString&) const;    //update to have the same definiton of place_into_board
    LString return_row_with(const LString&, int) const;
    int perpendicular_points(const LString&) const;           //counts all points for all vertical words created by a given horizontal word
    bool contains_letter_of_hand(const LString&) const;
    bool contains_letter_of_hand(const string& passed) const;

private:
    int bestHX, bestHY;
    LString bestHWord;               //holds the best horizontal word for the current board
    LString hand;                   //holds the hand of the solver
    vector<LString> board;          //holds the game board
    unordered_set<LString, MyHashFunction> answerSet;
    vector<LString> wordsOfRow[15];     //needs testing
};


#endif //SCRABBLEBOT_BOARDREADER_H
