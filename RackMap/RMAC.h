#ifndef SCRABBLE_SOLVER_RMAC_H
#define SCRABBLE_SOLVER_RMAC_H

#include "../CADS/CADS.h"

class RMAC {    //Rack Map And Calculator
public:
    RMAC();
    static void compute_and_print_rack_database(string, string);
    static void compute_and_print_sub_racks(string);
    vector<TString>& operator[](const string&);
    ~RMAC(){ delete data; }
private:
    unordered_map<string, vector<TString>>* data = nullptr;
};


#endif //SCRABBLE_SOLVER_RMAC_H
