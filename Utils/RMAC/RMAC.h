#ifndef SCRABBLE_SOLVER_RMAC_H
#define SCRABBLE_SOLVER_RMAC_H

#include "../../Utils/CADS/CADS.h"
#include "../Tstring/Tstring.h"
#include <unordered_set>
#include <string>
#include <vector>

namespace scl{
    struct RMAC {    //Rack Map And Calculator
        RMAC() = default;

        RMAC(const std::string &, const std::string &);

        RMAC(const std::string &, const std::unordered_set<std::string> &);

        /**
         * @brief Determines if the passed scl::Tstring is composed of Tiles from @param rackMap and the number of blanks tracked in @param blankCount
         */
        bool is_descendent_of(scl::Tstring& desc, const int* rackMap, int blankCount) const;

        std::string rack;
        std::vector<scl::Tstring> data;
    };
}


#endif //SCRABBLE_SOLVER_RMAC_H
