#ifndef SCRABBLE_SOLVER_CADS_H
#define SCRABBLE_SOLVER_CADS_H

#include <string>
#include <vector>

#include "../Tstring/Tstring.h"

namespace scl {
struct AnchoredString {
    AnchoredString(const std::string &str1, int subscript) {
        first = str1;
        second = subscript;
    }

    std::string first;
    int second;
};

class CADS {  // Coordinated Anchored Data Set
   public:
    CADS() { data = nullptr; }

    explicit CADS(const char *);

    CADS(const CADS &);

    CADS &operator=(const CADS &);

    /**
     * @brief Returns the list of words that have the passed char at the passed x value.
     */
    std::vector<AnchoredString> &at_with(int x, unsigned char toFind);

    /**
     * @brief Resets all member variables of this CADS instance.
     */
    CADS &clear_all();

    ~CADS();

   private:
    std::vector<AnchoredString> **data;
};

}  // namespace scl

#endif  // SCRABBLE_SOLVER_CADS_H
