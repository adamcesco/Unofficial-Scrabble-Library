#ifndef SCRABBLE_SOLVER_SCRABBLEVECTORIZER_H
#define SCRABBLE_SOLVER_SCRABBLEVECTORIZER_H

#include "../CADS/CADS.h"
#include "../RackMap/RMAC.h"

namespace scl{
    enum TYPE{HORIZONTAL, VERTICAL, UNDEFINED_TYPE};
    enum RMAC_ROUTE{DICTIONARY, FILEPATH, UNDEFINED_ROUTE};

    class ScrabbleVectorizer {
    public:
        ScrabbleVectorizer();

        explicit ScrabbleVectorizer(const std::string &passed);

        virtual void build_board_from(const char *) = 0;

        virtual void console_print_formatted_board() const = 0;

        virtual void validate_board() const = 0;

        virtual std::vector<std::string> return_formatted_char_board_copy() const = 0;

        virtual std::vector<scl::Tstring> return_formatted_board_copy() const = 0;

        virtual std::vector<std::string> return_formatted_perkBoard_copy() const = 0;

        virtual TYPE get_vectorizer_type() const = 0;

        virtual void build_board_from(const char **) = 0;

        virtual void build_perkBoard_from(const char **) = 0;

        virtual std::vector<scl::Tstring> &get_all_moves_at(int, int) = 0;

        virtual void guided_place(int, int, TYPE, const std::string&) = 0;

        virtual void validate_words() = 0;

        virtual void search_for_intersecting_words();

        virtual void search_for_tangential_words();

        virtual void raw_place_boarded_word(const scl::Tstring &);

        virtual void place_best_move_into_board();

        virtual int points_of_raw_boarded_tstr(
                const scl::Tstring &word) const;                          //assumes that the passed word is found within the board and has proper coordinate values for the given std::vectorizer type

        void clear_all_moves();

        scl::Tstring &get_best_raw_boarded_word() { return bestWord; }

        void set_rack(const std::string &passed) {
            rack = passed;
            sort(rack.begin(), rack.end());
        }

        std::vector<scl::Tstring> &get_raw_board() { return board; }

        std::vector<std::string> return_raw_char_board_copy();

        std::vector<std::string> return_raw_perkBoard_copy();

        int &get_best_x() { return bestX; }

        int &get_best_y() { return bestY; }

        void reset_all_data();

        std::string &get_rack() { return rack; }

        void set_raw_board(const std::vector<scl::Tstring> &passed) { board = passed; }

        std::unordered_set<std::string> &get_dictionary() { return dictionary; }

        std::unordered_set<std::string> &get_sub8_dictionary() { return dictionarySub8; }

        std::vector<scl::Tstring> **get_moveSets() { return moveSets; }

        void set_dictionary(const std::unordered_set<std::string> &passed) { dictionary = passed; }

        void set_sub8_dictionary(const std::unordered_set<std::string> &passed) { dictionarySub8 = passed; }

        void build_dictionaries_from(const char *);

        void build_CADS_from(const char *filePath) { wordDataset = CADS(filePath); }

        void set_RMAC_build_path(const std::string &filePath) {
            routeRMAC = FILEPATH;
            rmacFilePath = filePath;
        }

        void set_RMAC_build_path_dictionary() { routeRMAC = DICTIONARY; }

        void prep_perkBoard();

        bool contains_letter_of_rack(const scl::Tstring &) const;

        ~ScrabbleVectorizer();

    protected:
        virtual std::vector<scl::Tstring> return_raw_board_with(
                const scl::Tstring &) const;        //assumes that the passed word is formatted with respect to the current std::vectorizer type

        int bestX, bestY;
        scl::Tstring bestWord;
        std::string rack;
        RMAC_ROUTE routeRMAC = UNDEFINED_ROUTE;
        std::string rmacFilePath;
        std::unordered_set<std::string> dictionary;
        std::unordered_set<std::string> dictionarySub8;
        std::vector<scl::Tstring> **moveSets;
        std::vector<scl::Tstring> board;
        CADS wordDataset;
        char perkBoard[15][15] = {{'3', ' ', ' ', 'B', ' ', ' ', ' ', '3', ' ', ' ', ' ', 'B', ' ', ' ', '3'},
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
}


#endif //SCRABBLE_SOLVER_SCRABBLEVECTORIZER_H
