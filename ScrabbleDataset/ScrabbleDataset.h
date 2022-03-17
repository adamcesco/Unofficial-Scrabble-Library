#ifndef SCRABBLE_SOLVER_SCRABBLEDATASET_H
#define SCRABBLE_SOLVER_SCRABBLEDATASET_H

#include "../LString/LString.h"

struct AnchoredString{
    AnchoredString(const string& str1, int subscript){ first = str1; second = subscript; }
    string first;
    int second;
};

class ScrabbleDataset {
public:
    ScrabbleDataset();
    explicit ScrabbleDataset(string);
    vector<AnchoredString> return_this_at(int x, int y, char toFind){ return data[x][y][int(abs(toFind) & 31)]; }
    ~ScrabbleDataset();

private:
    vector<AnchoredString>*** data = nullptr;
};


#endif //SCRABBLE_SOLVER_SCRABBLEDATASET_H
