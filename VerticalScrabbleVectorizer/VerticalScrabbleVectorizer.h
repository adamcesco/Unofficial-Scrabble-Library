#ifndef SCRABBLE_SOLVER_VERTICALSCRABBLEVECTORIZER_H
#define SCRABBLE_SOLVER_VERTICALSCRABBLEVECTORIZER_H

#include "../ScrabbleVectorizer/ScrabbleVectorizer.h"

class VerticalScrabbleVectorizer: public ScrabbleVectorizer{
public:
    VerticalScrabbleVectorizer() : ScrabbleVectorizer(){};
    explicit VerticalScrabbleVectorizer(const string &passed) : ScrabbleVectorizer(passed){}

    void build_board_from(const char*);
    void console_print_formatted_board() const;
    string to_string() const;
    TString update_best_word();
    void validate_words();
    Type get_vectorizer_type() const{ return VERTICAL; }
    vector<TString> return_formatted_board_copy() const;
    void set_board(const vector<TString>&);                //assumes the passed board is oriented in a horizontal format
    void set_board(const vector<string>&);             //assumes the passed board is oriented in a horizontal format
    void set_perkBoard(const vector<string>&);
    void validate_board() const;
    int points_of_best_word() const{ return points_of_word(bestWord); }

    vector<TString>& get_all_moves_at(int x, int y) { return moveSets[14 - x][y]; } //add bounds checking

    vector<string> return_formatted_perkBoard_copy() const;
    vector<string> return_formatted_char_board_copy() const;
};


#endif //SCRABBLE_SOLVER_VERTICALSCRABBLEVECTORIZER_H
