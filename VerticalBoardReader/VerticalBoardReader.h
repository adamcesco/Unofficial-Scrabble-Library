//
// Created by misc1 on 3/3/2022.
//

#ifndef SCRABBLE_SOLVER_VERTICALBOARDREADER_H
#define SCRABBLE_SOLVER_VERTICALBOARDREADER_H
//#include "../ScrabbleReader/ScrabbleReader.h"
#include "../LString/LString.h"

class VerticalBoardReader{
public:
    VerticalBoardReader();
    explicit VerticalBoardReader(const LString&);

    void build_board();
    void print_board() const;
    string to_string() const;       //print the info of the best word for the current board
    LString update_best_word();
    void validate_words_perpendicular();
    Type get_reader_type() const{return VERTICAL;}
    vector<LString> return_word_set_of(int subscript){return wordSets[14 - subscript];}
    vector<LString> board_to_string() const;

    LString get_best_word() const{return bestWord;}
    int get_best_x() const{return bestX;}
    int get_best_y() const {return bestY;}
    void search_board_for_words();
    int points_of_best_word(){return perpendicular_points(bestWord) + bestWord.get_letter_points();}
    void reset_all_data();
    void set_hand(const LString& passed){hand = passed;}
    LString get_hand(){return hand;}
    void place_into_board(const LString&);
    void filter_scrabble_words_by_hand();
    vector<LString>& get_true_board(){return board;}

protected:
    vector<LString> return_board_with(const LString&) const;
    LString return_row_with(const LString&, int) const;
    int perpendicular_points(const LString&) const;
    bool contains_letter_of_hand(const LString&) const;
    bool contains_letter_of_hand(const string& passed) const;

private:
    int bestX, bestY;
    LString bestWord;
    LString hand;
    vector<LString> board;
    unordered_set<LString, MyHashFunction> answerSet;
    vector<LString> wordSets[15];
};


#endif //SCRABBLE_SOLVER_VERTICALBOARDREADER_H
