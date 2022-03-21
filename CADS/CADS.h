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
    vector<AnchoredString>& return_this_at(int x, char toFind){
        if(x > 14 || !isalpha(toFind))
            throw invalid_argument("Error in vector<AnchoredString> return_this_at(int, int, char) | Invalid "
                                   "parameter value.\nX: " + to_string(x) + "\nChar: " + toFind);
        return data[x][int(abs(toFind) & 31)]; }
    ~CADS();

private:
    vector<AnchoredString>** data = nullptr;   //all rows for any CADS are all the same
    //TODO: You could implement a CADS that is based off of Anchored-DAWGs instead of AnchoredStrings
};


#endif //SCRABBLE_SOLVER_CADS_H
