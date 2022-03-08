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

    void build_board(const string &);
    void print_formatted_board() const;
    string to_string() const;
    LString update_best_word();
    void validate_words();
    Type get_reader_type() const{return HORIZONTAL;}
    vector<LString> return_word_set_of(int subscript){return wordSets[subscript];}
    vector<LString> return_formatted_board() const{return board;}
    void set_board(vector<LString>);
    void validate_board() const;
    int points_of_best_word(){return points_of_word(bestWord);}

    vector<vector<LString>> return_formatted_wordSets() const;
    vector<string> return_formatted_perkBoard() const;
    vector<LString> return_formatted_board_with(const LString &) const;
    vector<string> return_formatted_char_board() const;
};

#endif //SCRABBLEBOT_BOARDREADER_H