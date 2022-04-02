#include "RMAC.h"

RMAC::RMAC(const string& pRack, const string& filePath) {
    rack = pRack;
    ifstream wordCorpus;
    wordCorpus.open(filePath);
    if(!wordCorpus.is_open())
        throw invalid_argument("Could not open file path passed to RMAC::RMAC(const string& pRack, const string& filePath)");

    string word;
    int count = 0;
    while(wordCorpus.good()) {
        getline(wordCorpus, word);
        count++;

        if(word.empty())
            continue;
        TString toPush = word;
        while(isspace(toPush.back().letter))
            toPush.pop_back();
        if(TString::is_descendent_of(toPush, rack)){
            int len = toPush.length() - 1;
            toPush.set_x_vals_to_subscripts();
            toPush.add_to_x_vals(0 - len);
            for (int i = 0; i < len; ++i) {
                toPush.add_to_x_vals(1);
                data.push_back(toPush);
            }
        }
    }
    wordCorpus.close();
}