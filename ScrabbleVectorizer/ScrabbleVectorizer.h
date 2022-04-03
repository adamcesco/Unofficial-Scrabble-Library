#ifndef SCRABBLE_SOLVER_SCRABBLEVECTORIZER_H
#define SCRABBLE_SOLVER_SCRABBLEVECTORIZER_H

#include "../CADS/CADS.h"
#include "../RackMap/RMAC.h"

enum TYPE{HORIZONTAL, VERTICAL, UNDEFINED_TYPE};
enum RMAC_ROUTE{DICTIONARY, FILEPATH, UNDEFINED_ROUTE};

class ScrabbleVectorizer {
public:
    ScrabbleVectorizer();
    explicit ScrabbleVectorizer(const string &passed);
    virtual void build_board_from(const char*) = 0;
    virtual void console_print_formatted_board() const = 0;
    virtual void validate_board() const = 0;
    virtual vector<string> return_formatted_char_board_copy() const = 0;
    virtual vector<TString> return_formatted_board_copy() const = 0;
    virtual vector<string> return_formatted_perkBoard_copy() const = 0;
    virtual TYPE get_vectorizer_type() const = 0;
    virtual void set_board(const char**) = 0;
    virtual void set_perkBoard(const char**) = 0;
    virtual vector<TString>& get_all_moves_at(int, int) = 0;

    virtual void search_for_intersecting_words();
    virtual void search_for_tangential_words();
    virtual void place_into_board(const TString&);
    virtual void place_best_word_into_board();
    virtual int points_of_placed_word(const TString &) const;                          //assumes that the passed word is found within the board and has proper coordinate values for the given vectorizer type

    void clear_all_moves();
    TString& get_best_word(){ return bestWord; }
    void set_rack(const string& passed){ rack = passed; sort(rack.begin(), rack.end()); }
    vector<TString>& get_raw_board(){ return board; }
    vector<string> return_raw_char_board_copy();
    vector<string> return_raw_perkBoard_copy();
    int& get_best_x() { return bestX; }
    int& get_best_y() { return bestY; }
    void reset_all_data();
    string& get_rack(){ return rack; }
    void set_raw_board(const vector<TString>& passed){ board = passed; }
    unordered_set<string>& get_dictionary(){ return dictionary; }
    unordered_set<string>& get_sub8_dictionary(){ return dictionarySub8; }
    vector<TString>** get_moveSets(){ return moveSets; }
    void set_dictionary(const unordered_set<string>& passed){ dictionary = passed; }
    void set_sub8_dictionary(const unordered_set<string>& passed){ dictionarySub8 = passed; }
    void build_dictionaries_from(const char*);
    void build_CADS_from(const char* filePath) { wordDataset = CADS(filePath); }
    void set_RMAC_build_path(const string& filePath) { routeRMAC = FILEPATH; rmacFilePath = filePath; }
    void set_RMAC_build_path_dictionary() { routeRMAC = DICTIONARY; }
    void prep_perk_board();
    bool contains_letter_of_rack(const TString&) const;

    ~ScrabbleVectorizer();

protected:
    virtual vector<TString> return_raw_board_with(const TString&) const;        //assumes that the passed word is formatted with respect to the current vectorizer type

    int bestX, bestY;
    TString bestWord;
    string rack;
    RMAC_ROUTE routeRMAC = UNDEFINED_ROUTE;
    string rmacFilePath;
    unordered_set<string> dictionary;
    unordered_set<string> dictionarySub8;
    vector<TString>** moveSets;
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
