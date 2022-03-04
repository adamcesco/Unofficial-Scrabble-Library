//
// Created by misc1 on 3/3/2022.
//

#ifndef SCRABBLE_SOLVER_VERTICALBOARDREADER_H
#define SCRABBLE_SOLVER_VERTICALBOARDREADER_H
#include "../LString/LString.h"

class VerticalBoardReader {
public:
    VerticalBoardReader();
    explicit VerticalBoardReader(const LString&);
    void build_board();
    void print_board() const;
    void search_board_for_words();  //find all horizontal words that fit into the board
    string to_string() const;       //print the info of the best word for the current board
    LString update_best_vir_word();

    void check_vir_words_perpendicular();
    LString get_best_vir_word() const{return bestVWord;}
    int get_best_vir_x() const{return bestVX;}
    int get_best_vir_y() const{return bestVY;}
    Type get_reader_type() {return VERTICAL;}
    int points_of_best_vir_word(){return perpendicular_points(bestVWord) + bestVWord.get_letter_points();}
    void reset_all_data();
    void set_hand(const LString& passed){hand = passed;}
    void place_into_board(const LString&);    //place a word (vertical or horizontal) onto the board, only the first Letter of the word needs to have its coordinates defined
    void filter_scrabble_words_by_hand();
    vector<LString> return_words_of_col(int);     //needs testing
    vector<LString> get_true_board(){return board;}

protected:
    vector<LString> return_board_with(const LString&) const;    //update to have the same definiton of place_into_board
    LString return_row_with(const LString&, int) const;
    int perpendicular_points(const LString&) const;           //counts all points for all vertical words created by a given horizontal word
    bool contains_letter_of_hand(const LString&) const;
    bool contains_letter_of_hand(const string& passed) const;

private:
    int bestVX, bestVY;
    LString bestVWord;               //holds the best vertical word for the current board
    LString hand;                   //holds the hand of the solver
    vector<LString> board;          //holds the game board
    unordered_set<LString, MyHashFunction> answerSet;
    vector<LString> wordsOfCol[15];     //needs testing
};


#endif //SCRABBLE_SOLVER_VERTICALBOARDREADER_H
