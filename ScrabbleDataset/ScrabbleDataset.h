#ifndef SCRABBLE_SOLVER_SCRABBLEDATASET_H
#define SCRABBLE_SOLVER_SCRABBLEDATASET_H

#include "../LString/LString.h"

class ScrabbleDataset {
public:
    ScrabbleDataset();
    explicit ScrabbleDataset(string);
    vector<string> return_this_at(int x, int y, char toFind){return data[x][y][abs(toFind) & 31];}
    ~ScrabbleDataset(){delete[] data;}

private:
    vector<string>*** data = nullptr;
};


#endif //SCRABBLE_SOLVER_SCRABBLEDATASET_H
