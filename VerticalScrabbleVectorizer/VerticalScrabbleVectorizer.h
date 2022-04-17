#ifndef SCRABBLE_SOLVER_VERTICALSCRABBLEVECTORIZER_H
#define SCRABBLE_SOLVER_VERTICALSCRABBLEVECTORIZER_H

#include "../ScrabbleVectorizer/ScrabbleVectorizer.h"

namespace scl{
    class VerticalScrabbleVectorizer : public ScrabbleVectorizer {
    public:
        VerticalScrabbleVectorizer() : ScrabbleVectorizer() {};

        explicit VerticalScrabbleVectorizer(const std::string &passed) : ScrabbleVectorizer(passed) {}

        void build_board_from(const char *);

        void console_print_formatted_board() const;

        std::string to_string() const;

        scl::Tstring update_best_word();

        void validate_words();

        TYPE get_vectorizer_type() const { return VERTICAL; }

        std::vector<scl::Tstring> return_formatted_board_copy() const;

        void set_board(const std::vector<std::string> &);             //assumes the passed board is oriented in a horizontal format
        void validate_board() const;

        int points_of_best_word() const { return points_of_placed_word(bestWord); }

        std::vector<scl::Tstring> &get_all_moves_at(int x, int y) { return moveSets[14 - x][y]; } //add bounds checking

        std::vector<std::string> return_formatted_perkBoard_copy() const;

        std::vector<std::string> return_formatted_char_board_copy() const;

        void build_board_from(const char **);

        void build_perkBoard_from(const char **);
    };
}


#endif //SCRABBLE_SOLVER_VERTICALSCRABBLEVECTORIZER_H
