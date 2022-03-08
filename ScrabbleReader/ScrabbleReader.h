//
// Created by misc1 on 3/3/2022.
//

#ifndef SCRABBLE_SOLVER_SCRABBLEREADER_H
#define SCRABBLE_SOLVER_SCRABBLEREADER_H
#include "../LString/LString.h"

class ScrabbleReader {
public:
    //TODO: remove some pure virtual methods that are too restricting for future users of the class
    virtual void build_board(const string &) = 0;
    virtual void print_formatted_board() const = 0;
    virtual Type get_reader_type() const = 0;
    virtual vector<LString> return_formatted_board() const = 0;
    virtual void set_board(vector<LString>) = 0;
    virtual void validate_board() const = 0;
    virtual vector<LString> return_formatted_board_with(const LString&) const = 0;      //assumes that the passed word is formatted horizontally
    virtual vector<string> return_formatted_char_board() const = 0;
    virtual vector<vector<LString>> return_formatted_wordSets() const = 0;
    virtual vector<string> return_formatted_perkBoard() const = 0;

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
    void place_into_board(const LString&);
    vector<LString>& get_raw_board(){return board;}
    void set_raw_board(vector<LString> passed){board = passed;}
    vector<LString> return_raw_board_with(const LString&) const;
    int points_of_word(const LString &);
    int find_points_of_word(const string&);
    void place_best_word_into_board();
    vector<string> return_raw_char_board();
    vector<string> return_raw_perkBoard();

protected:
    int perpendicular_points(const LString&) const;
    bool contains_letter_of_hand(const LString&) const;
    vector<LString> return_all_fitted_tangential_words(LString&, int);
    unordered_map<LString, LString, MyHashFunction> return_all_fitted_filled_rows(LString&, int);
    unordered_map<LString, LString, MyHashFunction> return_all_fitted_rows(LString&, int);

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
