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
    CADS() { data = nullptr; }
    explicit CADS(const char*);
    CADS(const CADS&);
    CADS& operator =(const CADS&);
    vector<AnchoredString>& at_with(int x, unsigned char toFind);
    CADS& clear_all();
    ~CADS();

private:
    vector<AnchoredString>** data;
    //TODO: You could implement a CADS that is based off of Anchored-TStrings instead of AnchoredStrings
    //TODO: You could implement a CADS that is based off of Anchored-DAWGs instead of AnchoredStrings
};


#endif //SCRABBLE_SOLVER_CADS_H
