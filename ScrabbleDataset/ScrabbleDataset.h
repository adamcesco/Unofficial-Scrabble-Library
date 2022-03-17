#ifndef SCRABBLE_SOLVER_SCRABBLEDATASET_H
#define SCRABBLE_SOLVER_SCRABBLEDATASET_H

#include "../LString/LString.h"

struct AnchoredSet{
    AnchoredSet(string str1, int subscript){first = str1; second = subscript;}
    string first;
    int second;
};

class ScrabbleDataset {
public:
    ScrabbleDataset();
    explicit ScrabbleDataset(string);
    vector<AnchoredSet> return_this_at(int x, int y, char toFind){return data[x][y][int(abs(toFind) & 31)];}
    ~ScrabbleDataset(){delete[] data;}

private:
    vector<AnchoredSet>*** data = nullptr;
};


#endif //SCRABBLE_SOLVER_SCRABBLEDATASET_H
