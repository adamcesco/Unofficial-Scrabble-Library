#ifndef SCRABBLEBOT_BOARDREADER_H
#define SCRABBLEBOT_BOARDREADER_H

#include "../ScrabbleVectorizer/ScrabbleVectorizer.h"

namespace scl{
    class HorizontalScrabbleVectorizer : public ScrabbleVectorizer {
    public:
        HorizontalScrabbleVectorizer() : ScrabbleVectorizer() {}

        explicit HorizontalScrabbleVectorizer(const std::string &passed) : ScrabbleVectorizer(passed) {}

        void build_board_from(const char *);

        void console_print_formatted_board() const;

        TYPE get_vectorizer_type() const { return HORIZONTAL; }

        std::vector<scl::Tstring> return_formatted_board_copy() const { return board; }

        void set_board(const std::vector<std::string> &);             //assumes the passed board is oriented in a horizontal format
        void validate_board() const;

        std::vector<std::string> return_formatted_perkBoard_copy() const;

        std::vector<std::string> return_formatted_char_board_copy() const;

        scl::Tstring update_best_move();

        std::vector<scl::Tstring> &get_all_moves_at(int x, int y) { return moveSets[y][x]; }

        std::string to_string() const;

        void validate_generated_moves();

        int points_of_best_boarded_move() const { return points_of_raw_boarded_tstr(bestWord); }

        void build_board_from(const char **);

        void build_perkBoard_from(const char **);

        virtual void guided_place(int x, int y, TYPE type, const scl::Tstring &);

        virtual void guided_place(int, int, TYPE, const std::string&);

    };
}

#endif //SCRABBLEBOT_BOARDREADER_H