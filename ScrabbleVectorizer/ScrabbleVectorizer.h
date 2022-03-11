#ifndef SCRABBLE_SOLVER_SCRABBLEVECTORIZER_H
#define SCRABBLE_SOLVER_SCRABBLEVECTORIZER_H
#include "../LString/LString.h"

class ScrabbleVectorizer {
public:
    //TODO: remove some pure virtual methods that are too restricting for future users of the class

    virtual void build_board(const string &) = 0;
    virtual void print_formatted_board() const = 0;
    virtual void validate_board() const = 0;
    virtual vector<string> return_formatted_char_board_copy() const = 0;
    virtual vector<string> return_formatted_perkBoard_copy() const = 0;
    virtual LString update_best_word() = 0;
    virtual vector<vector<LString>> return_formatted_answerSets_copy() const = 0;
    virtual vector<LString> return_formatted_board_copy() const = 0;
    virtual Type get_vectorizer_type() const = 0;
    virtual void set_board(const vector<string>&) = 0;
    virtual void set_perkBoard(const vector<string>&) = 0;

    void search_for_intersecting_words();
    void search_for_tangential_words();
    void search_for_all_words();
    void clear_wordSets();
    LString& get_best_word(){return bestWord;}
    void set_hand(const string& passed){hand = passed;}
    void place_into_board(const LString&);
    vector<LString>& get_raw_board(){return board;}
    vector<string> return_raw_char_board_copy();
    vector<string> return_raw_perkBoard_copy();
    int& get_best_x() {return bestX;}
    int& get_best_y() {return bestY;}
    void reset_all_data();
    string& get_hand(){return hand;}
    void set_raw_board(const vector<LString>& passed){board = passed;}
    unordered_set<LString, MyHashFunction>& get_all_scrabble_words(){ return scrabbleWordSet;}
    vector<int> find_points_of_word(const string&);
    vector<LString> return_all_of_raw_word(const string&);
    void place_best_word_into_board();
    vector<LString>* get_answerSets(){return answerSets;}
//    char** get_perkBoard(){return perkBoard;}

protected:
    int points_of_word(const LString &);                            //assumes that the passed word is found within the board and has proper coordinate values for the given vectorizer type
    vector<LString> return_raw_board_with(const LString&) const;    //assumes that the passed word is formatted with respect to the current vectorizer type
    int perpendicular_points(const LString&) const;
    bool contains_letter_of_hand(const LString&) const;
    vector<LString> return_all_fitted_tangential_words(LString&, int);
    unordered_map<LString, LString, MyHashFunction> return_all_fitted_filled_rows(LString&, int);
    unordered_map<LString, LString, MyHashFunction> return_all_fitted_rows(LString&, int);

    int bestX, bestY;
    LString bestWord;
    string hand;
    unordered_set<LString, MyHashFunction> scrabbleWordSet;
    vector<LString> answerSets[15];
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
                                        //2 = double word premium
                                        //3 = triple word premium
                                        //B = double letter premium
                                        //C = triple letter premium
};


#endif //SCRABBLE_SOLVER_SCRABBLEVECTORIZER_H
