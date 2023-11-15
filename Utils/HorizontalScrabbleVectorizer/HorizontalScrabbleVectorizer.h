#ifndef SCRABBLEBOT_BOARDREADER_H
#define SCRABBLEBOT_BOARDREADER_H

#include "../ScrabbleVectorizer/ScrabbleVectorizer.h"

namespace scl {

class HorizontalScrabbleVectorizer : public ScrabbleVectorizer {
   public:
    HorizontalScrabbleVectorizer() : ScrabbleVectorizer() {}

    explicit HorizontalScrabbleVectorizer(const std::string &passed) : ScrabbleVectorizer(passed) {}

    void build_board_from(const char *);

    void console_print_formatted_board() const;

    TYPE get_vectorizer_type() const { return HORIZONTAL; }

    std::vector<scl::ScrabbleString> return_formatted_board_copy() const { return board; }

    /**
     * @brief Builds the board of this vectorizer from the passed std::vector<std::string>.
     * @warning The passed std::vector<std::string> must be size 15 x 15.
     * @warning All chars within the passed std::vector<std::string> must be Uppercase
     * @warning This method assumes that the passed baord has proper formatting
     */
    void build_board_from(const std::vector<std::string> &);

    void validate_board() const;

    std::vector<std::string> return_formatted_perkBoard_copy() const;

    std::vector<std::string> return_formatted_char_board_copy() const;

    /**
     * @brief Searches the move-set of this vectorizer, then saves and returns the move with the highest points into the board. The point calculation of each move includes perks and the points generated from creating neighboring words
     */
    scl::ScrabbleString update_best_move();

    std::vector<scl::ScrabbleString> &get_all_moves_at(int x, int y) { return moveSets[y][x]; }

    void validate_generated_moves();

    /**
     * @brief Returns the points of the pre-calculated beest-move of this vectorizer, this point calculation includes perks and the points generated from creating neighboring words.
     * @warning The calculation of the best-move and it's coordinates needs to done before hand via "scl::ScrabbleWord update_best_move()".
     */
    int points_of_best_boarded_move() const { return points_of_raw_boarded_tstr(bestWord); }

    void build_board_from(const char **);

    void build_perkBoard_from(const char **);

    /**
     * @brief Places the passed scl::ScrabbleWord into the current board of this vectorizer at the passed coordinate, regardless of the coordinates within the scl::Tiles within the passed scl::ScrabbleWord.
     * @param type: States whether this move is a VERTICAL or HORIZONTAL move.
     * @warning After placing the passed string into the board, the perk-board may need to be prepared via "void update_perkBoard()".
     */
    virtual void guided_place(int x, int y, TYPE type, const scl::ScrabbleString &tstr);

    virtual void guided_place(int x, int y, TYPE type, const std::string &str);

    /**
     * @brief Returns the pre-calculated move within this vectorizers move-set that produces that highest amount of points.
     * @warning This method just returns the pre-calculated best-move of this vectorizer, the calculation of the best-move needs to done before hand via "scl::ScrabbleWord update_best_move()".
     */
    scl::ScrabbleString &get_best_raw_boarded_move() { return bestWord; }

    /**
     * @brief Returns the x coordinate/subscript of the pre-calculated best-move of this vectorizer.
     * @warning The calculation of the best-move and it's coordinates needs to done before hand via "scl::ScrabbleWord update_best_move()".
     */
    int &get_best_x() { return bestX; }

    /**
     * @brief Returns the y coordinate/subscript of the pre-calculated best-move of this vectorizer.
     * @warning The calculation of the best-move and it's coordinates needs to done before hand via "scl::ScrabbleWord update_best_move()".
     */
    int &get_best_y() { return bestY; }

    /**
     * @brief Places the move with the highest points into the board.
     * @warning The calculation of the best-move needs to done before hand via "scl::ScrabbleWord update_best_move()".
     * @attention After placing the "best-move" into the board, the perk-board DOES NOT need to be prepared via "void update_perkBoard()".
     */
    virtual void place_best_move_into_board();

   protected:
    int bestX, bestY;
    scl::ScrabbleString bestWord;
};
}  // namespace scl

#endif  // SCRABBLEBOT_BOARDREADER_H