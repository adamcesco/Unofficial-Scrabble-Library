//
// Created by misc1 on 3/3/2022.
//

#ifndef SCRABBLE_SOLVER_SCRABBLEREADER_H
#define SCRABBLE_SOLVER_SCRABBLEREADER_H
#include "../LString/LString.h"

class ScrabbleReader {
public:
    virtual void build_board() = 0;
    virtual void print_board() const = 0;
    virtual string to_string() const = 0;
    virtual LString update_best_word() = 0;
    virtual void validate_words_perpendicular() = 0;
    virtual Type get_reader_type() const = 0;
    virtual vector<LString> return_word_set_of(int) = 0;
    virtual vector<LString> board_to_string() const = 0;
    virtual void set_board(vector<LString>) = 0;
    virtual void validate_board() const = 0;

    LString get_best_word() const{return bestWord;}
    int get_best_x() const{return bestX;}
    int get_best_y() const {return bestY;}
    void search_board_for_words();
    int points_of_best_word(){return perpendicular_points(bestWord) + bestWord.get_letter_points();}
    void reset_all_data();
    void set_hand(const LString& passed){hand = passed;}
    LString get_hand(){return hand;}
    void place_into_board(const LString&);
    vector<LString>& get_true_board(){return board;}

protected:
    vector<LString> return_board_with(const LString&) const;
    int perpendicular_points(const LString&) const;
    bool contains_letter_of_hand(const LString&) const;
    bool contains_letter_of_hand(const string& passed) const;

    int bestX, bestY;
    LString bestWord;
    LString hand;
    vector<LString> board;
    unordered_set<LString, MyHashFunction> answerSet;
    vector<LString> wordSets[15];
};


#endif //SCRABBLE_SOLVER_SCRABBLEREADER_H
