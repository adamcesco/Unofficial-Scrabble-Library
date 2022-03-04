//
// Created by misc1 on 3/3/2022.
//

#ifndef SCRABBLE_SOLVER_SCRABBLEREADER_H
#define SCRABBLE_SOLVER_SCRABBLEREADER_H
#include "../LString/LString.h"

class ScrabbleReader {
public:
    ScrabbleReader();
    explicit ScrabbleReader(const LString&);
    void build_board();
    void print_board() const;
    void search_board_for_words();  //find all horizontal words that fit into the board
    string to_string() const;       //print the info of the best word for the current board
    LString update_best_hor_word();

    virtual void validate_words_perpendicular() = 0;
    virtual LString get_best_word() const = 0;
    virtual int get_best_x() const = 0;
    virtual int get_best_y() const = 0;
    virtual Type get_reader_type() const = 0;
    int points_of_best_hor_word(){return perpendicular_points(bestWord) + bestWord.get_letter_points();}
    void reset_all_data();
    void set_hand(const LString& passed){hand = passed;}
    void place_into_board(const LString&);    //place a word (vertical or horizontal) onto the board, only the first Letter of the word needs to have its coordinates defined
    void filter_scrabble_words_by_hand();
    virtual vector<LString> return_word_set_of(int) = 0;     //needs testing
    vector<LString> get_board(){return board;}
    virtual vector<LString> board_to_string() = 0;

protected:
    vector<LString> return_board_with(const LString&) const;    //update to have the same definiton of place_into_board
    LString return_row_with(const LString&, int) const;
    int perpendicular_points(const LString&) const;           //counts all points for all vertical words created by a given horizontal word
    bool contains_letter_of_hand(const LString&) const;
    bool contains_letter_of_hand(const string& passed) const;

private:
    int bestX, bestY;
    LString bestWord;               //holds the best horizontal word for the current board
    LString hand;                   //holds the hand of the solver
    vector<LString> board;          //holds the game board
    unordered_set<LString, MyHashFunction> answerSet;
    vector<LString> wordsSet[15];     //needs testing
};


#endif //SCRABBLE_SOLVER_SCRABBLEREADER_H
