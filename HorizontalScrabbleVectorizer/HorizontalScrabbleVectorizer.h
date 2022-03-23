#ifndef SCRABBLEBOT_BOARDREADER_H
#define SCRABBLEBOT_BOARDREADER_H

#include "../ScrabbleVectorizer/ScrabbleVectorizer.h"

class HorizontalScrabbleVectorizer : public ScrabbleVectorizer{
public:
    HorizontalScrabbleVectorizer();
    explicit HorizontalScrabbleVectorizer(const string &passed);

    void build_board(const string &);
    void console_print_formatted_board() const;
    Type get_vectorizer_type() const{ return HORIZONTAL; }
    vector<TString> return_formatted_board_copy() const{ return board; }
    void set_board(const vector<TString>&);            //assumes the passed board is oriented in a horizontal format
    void set_board(const vector<string>&);             //assumes the passed board is oriented in a horizontal format
    void set_perkBoard(const vector<string>&);
    void validate_board() const;
    vector<string> return_formatted_perkBoard_copy() const;
    vector<string> return_formatted_char_board_copy() const;
    TString update_best_word();

    string to_string() const;
    void validate_words();
    vector<TString>& return_answer_set_of(int subscript){ return moveSets[subscript]; }
    int points_of_best_word() const{ return points_of_word(bestWord); }
    vector<vector<TString>> return_formatted_answerSets_copy() const;
};

#endif //SCRABBLEBOT_BOARDREADER_H