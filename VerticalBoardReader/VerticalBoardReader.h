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

    void build_board();
    void print_board() const;
    string to_string() const;
    LString update_best_word();
    void validate_words_perpendicular();
    Type get_reader_type() const{return VERTICAL;}
    vector<LString> return_word_set_of(int subscript){return wordSets[subscript];}
    vector<LString> board_to_string() const{return board;}
    void set_board(vector<LString> passed);
    void validate_board() const;

};


#endif //SCRABBLE_SOLVER_VERTICALBOARDREADER_H
