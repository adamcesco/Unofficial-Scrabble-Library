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

    //does not support vertical words
    LString get_words_of_row(int);  //returns a LString that contains all found words that fit into the requested row
    LString row_to_string(int);     //returns a LString that contains all found words that fit into the requested row and each words points


    void check_perpendicular_compatibility();               //checks each horizontal word for each row to see if it is compatible with adjacent rows
    void to_vertical_reader();
    void to_horizontal_reader();
    LString get_best_hor_word() const{return bestHWord;}
    LString get_best_vir_word() const{return bestVWord;}
    int get_best_hor_x() const{return bestHX;}
    int get_best_hor_y() const{return bestHY;}
    int get_best_vir_x() const{return bestVX;}
    int get_best_vir_y() const{return bestVY;}
    Type get_reader_type() const{return readerType;}
    int points_of_best_hor_word(){return perpendicular_points(bestHWord) + bestHWord.get_horizontal_points();}
    int points_of_best_vir_word(){return perpendicular_points(bestVWord) + bestVWord.get_horizontal_points();}
    void reset_all_data();
    void prime_for_different_mode();
    void set_hand(const LString& passed){hand = passed;}
    void place_best_word_into_board();
    void place_into_board(const LString&, Type);    //place a word (vertical or horizontal) onto the board, only the first Letter of the word needs to have its coordinates defined

protected:
    vector<LString> return_board_with(const LString&) const;    //this returns a copy of the board that contains the passed LString
    int perpendicular_points(const LString&) const;           //counts all points for all vertical words created by a given horizontal word

private:
    int bestHX, bestHY;
    int bestVX, bestVY;
    LString bestVWord;               //holds the best vertical word for the current board
    LString bestHWord;               //holds the best horizontal word for the current board
    LString hand;                   //holds the hand of the solver
    Type readerType;                //states whether the best word for this BoardReader is a horizontal or a vertical reader
    vector<LString> board;          //holds the game board
    unordered_set<string> answerSet;
    vector<LString> wordsOfRow[15]; //an array of vectors, where each vector holds the words that fit into the given row
                                    // each row is sorted by myComp as an extension of answers being sorted by myComp
};


#endif //SCRABBLEBOT_BOARDREADER_H
