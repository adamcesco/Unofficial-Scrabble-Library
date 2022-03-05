//
// Created by misc1 on 2/26/2022.
//

#ifndef SCRABBLEBOT_BOARDREADER_H
#define SCRABBLEBOT_BOARDREADER_H

#include "../ScrabbleReader/ScrabbleReader.h"

class HorizontalBoardReader : public ScrabbleReader{
public:
    HorizontalBoardReader();
    explicit HorizontalBoardReader(const LString&);

    void build_board();
    void print_board() const;
    string to_string() const;
    LString update_best_word();
    void validate_words_perpendicular();
    Type get_reader_type() const{return HORIZONTAL;}
    vector<LString> return_word_set_of(int subscript){return wordSets[subscript];}
    vector<LString> board_to_string() const{return board;}
    void set_board(vector<LString> passed){board = passed;}
    void validate_board() const;
};


#endif //SCRABBLEBOT_BOARDREADER_H