#ifndef SCRABBLEBOT_BOARDREADER_H
#define SCRABBLEBOT_BOARDREADER_H

#include "../ScrabbleVectorizer/ScrabbleVectorizer.h"

namespace ssl{
    class HorizontalScrabbleVectorizer : public ScrabbleVectorizer {
    public:
        HorizontalScrabbleVectorizer() : ScrabbleVectorizer() {}

        explicit HorizontalScrabbleVectorizer(const std::string &passed) : ScrabbleVectorizer(passed) {}

        void build_board_from(const char *);

        void console_print_formatted_board() const;

        TYPE get_vectorizer_type() const { return HORIZONTAL; }

        std::vector<ssl::Tstring> return_formatted_board_copy() const { return board; }

        void set_board(const std::vector<std::string> &);             //assumes the passed board is oriented in a horizontal format
        void validate_board() const;

        std::vector<std::string> return_formatted_perkBoard_copy() const;

        std::vector<std::string> return_formatted_char_board_copy() const;

        ssl::Tstring update_best_word();

        std::vector<ssl::Tstring> &get_all_moves_at(int x, int y) { return moveSets[y][x]; }

        std::string to_string() const;

        void validate_words();

        int points_of_best_word() const { return points_of_placed_word(bestWord); }

        void build_board_from(const char **);

        void build_perkBoard_from(const char **);
    };
}

#endif //SCRABBLEBOT_BOARDREADER_H