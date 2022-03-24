#ifndef SCRABBLE_SOLVER_SCRABBLEVECTORIZER_H
#define SCRABBLE_SOLVER_SCRABBLEVECTORIZER_H

#include "../CADS/CADS.h"
#include "../RackMap/RMAC.h"

enum Type{HORIZONTAL, VERTICAL, UNDEFINED};

class ScrabbleVectorizer {
public:
    virtual void build_board(const string &) = 0;
    virtual void console_print_formatted_board() const = 0;
    virtual void validate_board() const = 0;
    virtual vector<string> return_formatted_char_board_copy() const = 0;
    virtual vector<string> return_formatted_perkBoard_copy() const = 0;
    virtual TString update_best_word() = 0;
    virtual vector<vector<TString>> return_formatted_answerSets_copy() const = 0;
    virtual vector<TString> return_formatted_board_copy() const = 0;
    virtual Type get_vectorizer_type() const = 0;
    virtual void set_board(const vector<string>&) = 0;
    virtual void set_perkBoard(const vector<string>&) = 0;

    virtual void search_for_intersecting_words();
    virtual void search_for_tangential_words();
    virtual void place_into_board(const TString&);
    virtual void place_best_word_into_board();

    void clear_wordSets();
    TString& get_best_word(){ return bestWord; }
    void set_hand(const string& passed){ rack = passed; sort(rack.begin(), rack.end()); }
    vector<TString>& get_raw_board(){ return board; }
    vector<string> return_raw_char_board_copy();
    vector<string> return_raw_perkBoard_copy();
    int& get_best_x() { return bestX; }
    int& get_best_y() { return bestY; }
    void reset_all_data();
    string& get_hand(){ return rack; }
    void set_raw_board(const vector<TString>& passed){ board = passed; }
    unordered_set<string>& get_all_scrabble_words(){ return scrabbleWordSet; }
    vector<int> find_points_of_word(const string&) const;
    vector<TString> return_all_of_raw_word(const string&) const;
    vector<TString>* get_moveSets(){ return moveSets; }

protected:
    virtual int points_of_word(const TString &) const;                          //assumes that the passed word is found within the board and has proper coordinate values for the given vectorizer type
    virtual vector<TString> return_raw_board_with(const TString&) const;        //assumes that the passed word is formatted with respect to the current vectorizer type
    bool contains_letter_of_hand(const TString&) const;

    int bestX, bestY;
    TString bestWord;
    string rack;
    unordered_set<string> scrabbleWordSet;
    vector<TString> moveSets[15];
    vector<TString> board;
    CADS wordDataset;
    char perkBoard[15][15] =   {{'3', ' ', ' ', 'B', ' ', ' ', ' ', '3', ' ', ' ', ' ', 'B', ' ', ' ', '3'},
                                {' ', '2', ' ', ' ', ' ', 'C', ' ', ' ', ' ', 'C', ' ', ' ', ' ', '2', ' '},
                                {' ', ' ', '2', ' ', ' ', ' ', 'B', ' ', 'B', ' ', ' ', ' ', '2', ' ', ' '},
                                {'B', ' ', ' ', '2', ' ', ' ', ' ', 'B', ' ', ' ', ' ', '2', ' ', ' ', 'B'},
                                {' ', ' ', ' ', ' ', '2', ' ', ' ', ' ', ' ', ' ', '2', ' ', ' ', ' ', ' '},
                                {' ', 'C', ' ', ' ', ' ', 'C', ' ', ' ', ' ', 'C', ' ', ' ', ' ', 'C', ' '},
                                {' ', ' ', 'B', ' ', ' ', ' ', 'B', ' ', 'B', ' ', ' ', ' ', 'B', ' ', ' '},
                                {'3', ' ', ' ', 'B', ' ', ' ', ' ', '2', ' ', ' ', ' ', 'B', ' ', ' ', '3'},
                                {' ', ' ', 'B', ' ', ' ', ' ', 'B', ' ', 'B', ' ', ' ', ' ', 'B', ' ', ' '},
                                {' ', 'C', ' ', ' ', ' ', 'C', ' ', ' ', ' ', 'C', ' ', ' ', ' ', 'C', ' '},
                                {' ', ' ', ' ', ' ', '2', ' ', ' ', ' ', ' ', ' ', '2', ' ', ' ', ' ', ' '},
                                {'B', ' ', ' ', '2', ' ', ' ', ' ', 'B', ' ', ' ', ' ', '2', ' ', ' ', 'B'},
                                {' ', ' ', '2', ' ', ' ', ' ', 'B', ' ', 'B', ' ', ' ', ' ', '2', ' ', ' '},
                                {' ', '2', ' ', ' ', ' ', 'C', ' ', ' ', ' ', 'C', ' ', ' ', ' ', '2', ' '},
                                {'3', ' ', ' ', 'B', ' ', ' ', ' ', '3', ' ', ' ', ' ', 'B', ' ', ' ', '3'}};
                                        //2 = double word premium
                                        //3 = triple word premium
                                        //B = double letter premium
                                        //C = triple letter premium
};


#endif //SCRABBLE_SOLVER_SCRABBLEVECTORIZER_H
