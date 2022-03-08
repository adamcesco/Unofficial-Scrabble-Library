//
// Created by misc1 on 3/3/2022.
//

#ifndef SCRABBLE_SOLVER_VERTICALBOARDREADER_H
#define SCRABBLE_SOLVER_VERTICALBOARDREADER_H

#include "../ScrabbleReader/ScrabbleReader.h"

class VerticalBoardReader: public ScrabbleReader{
public:
    VerticalBoardReader();
    explicit VerticalBoardReader(const LString&);

    void build_board(const string &);
    void print_formatted_board() const;
    string to_string() const;
    LString update_best_word();
    void validate_words();
    Type get_reader_type() const{return VERTICAL;}
    vector<LString> return_word_set_of(int subscript){return wordSets[14 - subscript];}
    vector<LString> return_formatted_board() const;
    void set_board(vector<LString>);
    void validate_board() const;
    int points_of_best_word(){return points_of_word(bestWord);}

    vector<vector<LString>> return_formatted_wordSets() const;
    vector<string> return_formatted_perkBoard() const;
    vector<LString> return_formatted_board_with(const LString &) const;
    vector<string> return_formatted_char_board() const;
};


#endif //SCRABBLE_SOLVER_VERTICALBOARDREADER_H
