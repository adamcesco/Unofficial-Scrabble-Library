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
    void print_foramtted_board() const;
    string to_string() const;
    LString update_best_word();
    void validate_words();
    Type get_reader_type() const{return VERTICAL;}
    vector<LString> return_word_set_of(int subscript){return wordSets[subscript];}
    vector<LString> return_formatted_board() const{return board;}
    void set_board(vector<LString>);
    void validate_board() const;
    int points_of_best_word(){return perpendicular_points(bestWord) + bestWord.get_letter_points();}
};


#endif //SCRABBLE_SOLVER_VERTICALBOARDREADER_H
