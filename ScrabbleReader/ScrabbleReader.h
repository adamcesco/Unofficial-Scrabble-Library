//
// Created by misc1 on 3/3/2022.
//

#ifndef SCRABBLE_SOLVER_SCRABBLEREADER_H
#define SCRABBLE_SOLVER_SCRABBLEREADER_H
#include "../LString/LString.h"

class ScrabbleReader {
public:
    virtual void build_board() = 0;
    virtual void print_formatted_board() const = 0;
    virtual Type get_reader_type() const = 0;
    virtual vector<LString> return_formatted_board() const = 0;     //needs testing
    virtual void set_board(vector<LString>) = 0;
    virtual void validate_board() const = 0;
    virtual vector<LString> return_formatted_board_with(const LString &) const = 0;     //needs testing
    virtual char** return_formatted_char_board(char**) const = 0;                       //needs testing
    virtual vector<LString>* return_formatted_wordSets(vector<LString>* ) const = 0;    //needs testing
    virtual char** return_formatted_perkBoard(char**) const = 0;                        //needs testing

    LString get_best_word() const{return bestWord;}
    int get_best_x() const{return bestX;}
    int get_best_y() const {return bestY;}
    void search_for_intersecting_words();
    void search_for_tangential_words();
    void search_for_all_words();
    void clear_wordSets();
    void reset_all_data();
    void set_hand(const LString& passed){hand = passed;}
    LString get_hand(){return hand;}
    void place_into_board(const LString &toPrint);
    vector<LString>& get_raw_board(){return board;}
    void set_raw_board(vector<LString> passed){board = passed;}
    vector<LString> return_raw_board_with(const LString &toPrint) const;
    int points_of_word(const LString &);
    int find_points_of_word(const string&);
    void place_best_word_into_board();
    char **return_raw_char_board(char **);  //needs testing
    char** return_raw_perkBoard(char **);   //needs testing

protected:
    int perpendicular_points(const LString&) const;
    bool contains_letter_of_hand(const LString&) const;
    bool contains_letter_of_hand(const string& passed) const;
    vector<LString> return_all_fitted_tangential_words(LString &word, int rowSubscript);
    unordered_map<LString, LString, MyHashFunction> return_all_fitted_filled_rows(LString &word, int rowSubscript);
    unordered_map<LString, LString, MyHashFunction> return_all_fitted_rows(LString &word, int rowSubscript);

    int bestX, bestY;
    LString bestWord;
    LString hand;
    vector<LString> board;
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
    unordered_set<LString, MyHashFunction> answerSet;
    vector<LString> wordSets[15];
};


#endif //SCRABBLE_SOLVER_SCRABBLEREADER_H
