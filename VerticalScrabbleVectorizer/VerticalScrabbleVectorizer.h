//
// Created by misc1 on 3/3/2022.
//

#ifndef SCRABBLE_SOLVER_VERTICALSCRABBLEVECTORIZER_H
#define SCRABBLE_SOLVER_VERTICALSCRABBLEVECTORIZER_H

#include "../ScrabbleVectorizer/ScrabbleVectorizer.h"

class VerticalScrabbleVectorizer: public ScrabbleVectorizer{
public:
    VerticalScrabbleVectorizer();
    explicit VerticalScrabbleVectorizer(const string &passed);

    void build_board(const string &);
    void print_formatted_board() const;
    string to_string() const;
    LString update_best_word();
    void validate_words();
    Type get_vectorizer_type() const{return VERTICAL;}
    vector<LString> return_word_set_of(int subscript){return answerSets[14 - subscript];}
    vector<LString> return_formatted_board() const;
    void set_board(vector<LString>);                //assumes the passed board is oriented in a horizontal format
    void validate_board() const;
    int points_of_best_word(){return points_of_word(bestWord);}

    vector<vector<LString>> return_formatted_answerSets() const;
    vector<string> return_formatted_perkBoard() const;
    vector<LString> return_formatted_board_with(const LString &) const;     //assumes that the passed word is formatted with respect to a horizontal board
    vector<string> return_formatted_char_board() const;
};


#endif //SCRABBLE_SOLVER_VERTICALSCRABBLEVECTORIZER_H
