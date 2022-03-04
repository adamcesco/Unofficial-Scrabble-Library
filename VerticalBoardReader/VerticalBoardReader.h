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
    void build_board() override;
    void print_board() const override;
    string to_string() const override;       //print the info of the best word for the current board
    LString update_best_word() override;
    vector<LString> board_to_string() const override;

    void validate_words_perpendicular() override;
    Type get_reader_type() const override{return VERTICAL;}
    vector<LString> return_word_set_of(int subscript) override{return wordSets[14 - subscript];}
    vector<LString> get_true_board(){return board;}
};


#endif //SCRABBLE_SOLVER_VERTICALBOARDREADER_H
