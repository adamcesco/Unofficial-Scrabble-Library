//
// Created by misc1 on 2/26/2022.
//

#ifndef SCRABBLEBOT_BOARDREADER_H
#define SCRABBLEBOT_BOARDREADER_H
#include "../LString/LString.h"

enum Type{HORIZONTAL, VERTICAL, UNDEFINED};

class BoardReader {
public:
    BoardReader();
    explicit BoardReader(const LString&);
    void build_board();
    void print_board() const;
    void search_board_for_words();  //find all horizontal words that fit into the board
    string to_string() const;       //print the info of the best word for the current board
    LString update_best_vir_word();
    LString update_best_hor_word();

    void check_hor_words_perpendicular();               //checks each horizontal word for each row to see if it is compatible with adjacent rows
    void check_vir_words_perpendicular();
    void to_vertical_reader();
    void to_horizontal_reader();
    LString get_best_hor_word() const{return bestHWord;}
    LString get_best_vir_word() const{return bestVWord;}
    int get_best_hor_x() const{return bestHX;}
    int get_best_hor_y() const{return bestHY;}
    int get_best_vir_x() const{return bestVX;}
    int get_best_vir_y() const{return bestVY;}
    Type get_reader_type() const{return readerType;}
    int points_of_best_hor_word(){return perpendicular_points(bestHWord, HORIZONTAL) + bestHWord.get_letter_points();}
    int points_of_best_vir_word(){return perpendicular_points(bestVWord, VERTICAL) + bestVWord.get_letter_points();}
    void reset_all_data();
    void prime_for_different_mode();
    void set_hand(const LString& passed){hand = passed;}
    void place_best_word_into_board();
    void place_into_board(const LString&, Type);    //place a word (vertical or horizontal) onto the board, only the first Letter of the word needs to have its coordinates defined
    void filter_scrabble_words_by_hand();
    vector<LString> return_words_of_row(int);     //needs testing
    vector<LString> return_words_of_col(int);     //needs testing
    vector<LString> get_board(){return board;}

protected:
    vector<LString> return_board_with(const LString&, Type) const;    //update to have the same definiton of place_into_board
    LString return_row_with(const LString&, int) const;
    int perpendicular_points(const LString&, Type) const;           //counts all points for all vertical words created by a given horizontal word
    bool contains_letter_of_hand(const LString&) const;
    bool contains_letter_of_hand(const string& passed) const;

private:
    int bestHX, bestHY;
    int bestVX, bestVY;
    LString bestVWord;               //holds the best vertical word for the current board
    LString bestHWord;               //holds the best horizontal word for the current board
    LString hand;                   //holds the hand of the solver
    Type readerType;                //states whether the best word for this BoardReader is a horizontal or a vertical reader
    vector<LString> board;          //holds the game board
    vector<LString> rotatedBoard;          //holds the game board
    unordered_set<LString, MyHashFunction> answerSet;
    vector<LString> wordsOfRow[15];     //needs testing
    vector<LString> wordsOfCol[15];     //needs testing
};


#endif //SCRABBLEBOT_BOARDREADER_H
