#ifndef SCRABBLE_SOLVER_RMAC_H
#define SCRABBLE_SOLVER_RMAC_H

#include "../CADS/CADS.h"

struct RMAC {    //Rack Map And Calculator
    RMAC() = default;
    RMAC(const string&, const string&);
    RMAC(const string&, const unordered_set<string>&);
    bool is_descendent_of(TString&, const int*, int) const;
    string rack;
    vector<TString> data;
};


#endif //SCRABBLE_SOLVER_RMAC_H
