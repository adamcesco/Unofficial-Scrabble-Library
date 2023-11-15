#include "RMAC.h"

#include <fstream>
#include <cassert>

scl::RMAC::RMAC(const std::string& pRack, const std::string& filePath) {
    std::ifstream wordCorpus;
    wordCorpus.open(filePath);
    assert(wordCorpus.is_open());

    rack = pRack;
    int rackMap[28];
    int blankCount = 0;
    for (int& i : rackMap)
        i = 0;
    for (char it : rack) {
        if (it == '?')
            blankCount++;
        rackMap[abs(it) - 63]++;
    }

    while (wordCorpus.good()) {
        std::string word;
        getline(wordCorpus, word);
        
        // get rid of trailing whitespace characters from word
        while (isspace(word.back())){
            word.pop_back();
        }
        if (word.empty()){
            continue;
        }
        
        scl::ScrabbleString toPush(word);
        if (is_descendent_of(toPush, rackMap, blankCount)) {
            int len = toPush.length() - 1;
            toPush.SetXValsToSubscripts();
            toPush.AddToXVals(0 - len);
            for (int i = 0; i < len; ++i) {
                toPush.AddToXVals(1);
                data.push_back(toPush);
            }
        }
    }
    wordCorpus.close();
}

scl::RMAC::RMAC(const std::string& pRack, const std::unordered_set<std::string>& dictionary) {
    rack = pRack;
    int rackMap[28];
    int blankCount = 0;
    for (int& i : rackMap)
        i = 0;
    for (char it : rack) {
        if (it == '?')
            blankCount++;
        rackMap[abs(it) - 63]++;
    }

    for (const auto& word : dictionary) {
        scl::ScrabbleString toPush(word);
        if (is_descendent_of(toPush, rackMap, blankCount)) {
            int len = toPush.length() - 1;
            toPush.SetXValsToSubscripts();
            toPush.AddToXVals(0 - len);
            for (int i = 0; i < len; ++i) {
                toPush.AddToXVals(1);
                data.push_back(toPush);
            }
        }
    }
}

bool scl::RMAC::is_descendent_of(scl::ScrabbleString& desc, const int* rackMap, int blankCount) const {
    int slen = desc.length();
    int rlen = rack.length();
    if (rlen == 0 || slen > rlen)
        return false;

    int descMap[28];
    for (int& i : descMap)
        i = 0;

    for (int i = 0; i < slen; ++i)
        descMap[desc[i].letter - 63]++;

    for (int i = 0; i < slen; ++i) {
        desc[i].x = i;
        int index = desc[i].letter - 63;
        bool RLC = rackMap[index] < descMap[index];
        if (blankCount == 0 && RLC)
            return false;
        else if (RLC) {
            desc[i].points = 0;
            blankCount--;
            descMap[index]--;
            i--;
        }
    }

    return true;
}
