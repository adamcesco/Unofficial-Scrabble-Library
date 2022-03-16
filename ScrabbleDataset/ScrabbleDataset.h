#ifndef SCRABBLE_SOLVER_SCRABBLEDATASET_H
#define SCRABBLE_SOLVER_SCRABBLEDATASET_H

#include "../LString/LString.h"

class ScrabbleDataset {
public:
    ScrabbleDataset();
private:
    vector<LString> data[1366];
};


#endif //SCRABBLE_SOLVER_SCRABBLEDATASET_H
