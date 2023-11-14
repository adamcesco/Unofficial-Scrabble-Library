#ifndef SCRABBLE_SOLVER_RMAC_H
#define SCRABBLE_SOLVER_RMAC_H

#include <string>
#include <unordered_set>
#include <vector>

#include "../ScrabbleString/ScrabbleString.h"

namespace scl {
struct RMAC {  // Rack Map And Calculator
    RMAC() = default;

    RMAC(const std::string &, const std::string &);

    RMAC(const std::string &, const std::unordered_set<std::string> &);

    /**
     * @brief Determines if the passed scl::ScrabbleWord is composed of Tiles from @param rackMap and the number of blanks tracked in @param blankCount
     */
    bool is_descendent_of(scl::ScrabbleString &desc, const int *rackMap, int blankCount) const;

    std::string rack;
    std::vector<scl::ScrabbleString> data;
};
}  // namespace scl

#endif  // SCRABBLE_SOLVER_RMAC_H
