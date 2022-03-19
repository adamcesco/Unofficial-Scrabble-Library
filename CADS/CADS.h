#ifndef SCRABBLE_SOLVER_CADS_H
#define SCRABBLE_SOLVER_CADS_H

#include "../TString/TString.h"

struct AnchoredString{
    AnchoredString(const string& str1, int subscript){ first = str1; second = subscript; }
    string first;
    int second;
};

class CADS {    //Coordinated Anchored Data Set
public:
    CADS();
    explicit CADS(string);
    vector<AnchoredString>& return_this_at(int x, int y, char toFind){
        if(x > 14 || y > 14 || !isalpha(toFind))
            throw invalid_argument("Error in vector<AnchoredString> return_this_at(int x, int y, char toFind) | Invalid "
                                   "parameter value.\nX: " + to_string(x) + "\nY: " + to_string(y) + "\nChar: " + toFind);
        return data[x][y][int(abs(toFind) & 31)]; }
    ~CADS();

private:
    vector<AnchoredString>*** data = nullptr;   //all rows for any CADS are all the same
};


#endif //SCRABBLE_SOLVER_CADS_H
