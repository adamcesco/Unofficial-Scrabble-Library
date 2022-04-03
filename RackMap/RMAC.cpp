#include "RMAC.h"

RMAC::RMAC(const string& pRack, const string& filePath) {
    ifstream wordCorpus;
    wordCorpus.open(filePath);
    if(!wordCorpus.is_open())
        throw invalid_argument("Could not open file path passed to RMAC::RMAC(const string& pRack, const string& filePath)");

    rack = pRack;
    int rackMap[28];
    int blankCount = 0;
    for (int & i : rackMap)
        i = 0;
    for (char it : rack) {
        if(it == '?')
            blankCount++;
        rackMap[abs(it) - 63]++;
    }

    string word;
    while(wordCorpus.good()) {
        getline(wordCorpus, word);

        if(word.empty())
            continue;
        TString toPush = word;
        while(isspace(toPush.back().letter))
            toPush.pop_back();
        if(is_descendent_of(toPush, rackMap, blankCount)){
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

RMAC::RMAC(const string &pRack, const unordered_set<string>& dictionary) {
    rack = pRack;
    int rackMap[28];
    int blankCount = 0;
    for (int & i : rackMap)
        i = 0;
    for (char it : rack) {
        if(it == '?')
            blankCount++;
        rackMap[abs(it) - 63]++;
    }

    for (const auto& word : dictionary) {
        TString toPush = word;
        if(is_descendent_of(toPush, rackMap, blankCount)){
            int len = toPush.length() - 1;
            toPush.set_x_vals_to_subscripts();
            toPush.add_to_x_vals(0 - len);
            for (int i = 0; i < len; ++i) {
                toPush.add_to_x_vals(1);
                data.push_back(toPush);
            }
        }
    }
}

bool RMAC::is_descendent_of(TString& sub, const int* rackMap, int blankCount) const {
    int slen = sub.length();
    int rlen = rack.length();
    if(rlen == 0 || slen > rlen)
        return false;

    int lCount[28];
    for (int & i : lCount)
        i = 0;

    for (int i = 0; i < slen; ++i)
        lCount[sub[i].letter - 63]++;

    for (int i = 0; i < slen; ++i) {
        sub[i].x = i;
        int index = sub[i].letter - 63;
        bool RLC = rackMap[index] < lCount[index];
        if(blankCount == 0 && RLC)
            return false;
        else if (RLC) {
            sub[i].points = 0;
            sub[i].isBlank = true;
            blankCount--;
            lCount[index]--;
            i--;
        }
    }

    return true;
}
