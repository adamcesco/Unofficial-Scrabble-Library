#ifndef SCRABBLE_SOLVER_SCRABBLEDATASET_H
#define SCRABBLE_SOLVER_SCRABBLEDATASET_H

#include "../LString/LString.h"

class ScrabbleDataset {
public:
    explicit ScrabbleDataset(string);
    vector<string> return_this_at(char toFind, int subscript){return data[(abs(toFind) * 15) + subscript];}
    ~ScrabbleDataset(){delete[] data;}

private:
    vector<string>* data = nullptr;
};


#endif //SCRABBLE_SOLVER_SCRABBLEDATASET_H
