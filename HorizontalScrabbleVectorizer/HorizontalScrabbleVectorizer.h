#ifndef SCRABBLEBOT_BOARDREADER_H
#define SCRABBLEBOT_BOARDREADER_H

#include "../ScrabbleVectorizer/ScrabbleVectorizer.h"

class HorizontalScrabbleVectorizer : public ScrabbleVectorizer{
public:
    HorizontalScrabbleVectorizer() : ScrabbleVectorizer(){}
    explicit HorizontalScrabbleVectorizer(const string &passed) : ScrabbleVectorizer(passed){}

    void build_board_from(const char*);
    void console_print_formatted_board() const;
    TYPE get_vectorizer_type() const{ return HORIZONTAL; }
    vector<TString> return_formatted_board_copy() const{ return board; }
    void set_board(const vector<string>&);             //assumes the passed board is oriented in a horizontal format
    void validate_board() const;
    vector<string> return_formatted_perkBoard_copy() const;
    vector<string> return_formatted_char_board_copy() const;
    TString update_best_word();

    vector<TString>& get_all_moves_at(int x, int y) { return moveSets[y][x]; }

    string to_string() const;
    void validate_words();
    int points_of_best_word() const{ return points_of_placed_word(bestWord); }

    void build_board_from(const char**);
    void build_perkBoard_from(const char**);
};

#endif //SCRABBLEBOT_BOARDREADER_H