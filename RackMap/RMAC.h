#ifndef SCRABBLE_SOLVER_RMAC_H
#define SCRABBLE_SOLVER_RMAC_H

#include "../CADS/CADS.h"

struct RMAC {    //Rack Map And Calculator
    RMAC(const string&, const string&);
    string rack;
    vector<TString> data;
};


#endif //SCRABBLE_SOLVER_RMAC_H
