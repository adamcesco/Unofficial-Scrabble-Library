#ifndef SCRABBLE_SOLVER_RACKMAP_H
#define SCRABBLE_SOLVER_RACKMAP_H

#include "../CADS/CADS.h"

class RackMap {
public:
    RackMap();
    vector<TString> & return_this_at(int, const string&);
    ~RackMap();
private:
    vector<TString>*** data = nullptr;
};


#endif //SCRABBLE_SOLVER_RACKMAP_H
