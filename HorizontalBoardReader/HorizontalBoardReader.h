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
    void print_foramtted_board() const;
    string to_string() const;
    LString update_best_word();
    void validate_words();
    Type get_reader_type() const{return HORIZONTAL;}
    vector<LString> return_word_set_of(int subscript){return wordSets[subscript];}
    vector<LString> return_formatted_board() const{return board;}
    void set_board(vector<LString> passed){board = passed;}
    void validate_board() const;
    int points_of_best_word(){return perpendicular_points(bestWord) + bestWord.get_letter_points();}
};


#endif //SCRABBLEBOT_BOARDREADER_H