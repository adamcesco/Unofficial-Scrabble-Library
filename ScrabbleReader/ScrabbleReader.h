//
// Created by misc1 on 3/3/2022.
//

#ifndef SCRABBLE_SOLVER_SCRABBLEREADER_H
#define SCRABBLE_SOLVER_SCRABBLEREADER_H
#include "../LString/LString.h"

class ScrabbleReader {
public:
    virtual void build_board() = 0;
    virtual void print_foramtted_board() const = 0;
    virtual Type get_reader_type() const = 0;
    virtual vector<LString> return_formatted_board() const = 0;
    virtual void set_board(vector<LString>) = 0;
    virtual void validate_board() const = 0;
    virtual int points_of_best_word() = 0;

    LString get_best_word() const{return bestWord;}
    int get_best_x() const{return bestX;}
    int get_best_y() const {return bestY;}
    void search_for_intersecting_words();
    void search_for_tangential_words();
    void search_for_all_words();
    void clear_wordSets();
    void reset_all_data();
    void set_hand(const LString& passed){hand = passed;}
    LString get_hand(){return hand;}
    void place_into_board(const LString &toPrint);
    vector<LString>& get_raw_board(){return board;}
    vector<LString> return_raw_board_with(const LString &toPrint) const;

protected:
    int perpendicular_points(const LString&) const;
    bool contains_letter_of_hand(const LString&) const;
    bool contains_letter_of_hand(const string& passed) const;
    vector<LString> return_all_fitted_tangential_words(LString &word, int rowSubscript);
    unordered_map<LString, LString, MyHashFunction> return_all_fitted_filled_rows(LString &word, int rowSubscript);
    unordered_map<LString, LString, MyHashFunction> return_all_fitted_rows(LString &word, int rowSubscript);

    int bestX, bestY;
    LString bestWord;
    LString hand;
    vector<LString> board;
    unordered_set<LString, MyHashFunction> answerSet;
    vector<LString> wordSets[15];
};


#endif //SCRABBLE_SOLVER_SCRABBLEREADER_H
