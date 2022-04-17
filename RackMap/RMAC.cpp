#include "RMAC.h"

RMAC::RMAC(const std::string& pRack, const std::string& filePath) {
    std::ifstream wordCorpus;
    wordCorpus.open(filePath);
    if(!wordCorpus.is_open())
        throw std::invalid_argument("Could not open file path passed to RMAC::RMAC(const string& pRack, const string& filePath)");

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

    std::string word;
    while(wordCorpus.good()) {
        getline(wordCorpus, word);

        if(word.empty())
            continue;
        ssl::Tstring toPush = word;
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

RMAC::RMAC(const std::string &pRack, const std::unordered_set<std::string>& dictionary) {
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
        ssl::Tstring toPush = word;
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

bool RMAC::is_descendent_of(ssl::Tstring& desc, const int* rackMap, int blankCount) const {
    int slen = desc.length();
    int rlen = rack.length();
    if(rlen == 0 || slen > rlen)
        return false;

    int descMap[28];
    for (int & i : descMap)
        i = 0;

    for (int i = 0; i < slen; ++i)
        descMap[desc[i].letter - 63]++;

    for (int i = 0; i < slen; ++i) {
        desc[i].x = i;
        int index = desc[i].letter - 63;
        bool RLC = rackMap[index] < descMap[index];
        if(blankCount == 0 && RLC)
            return false;
        else if (RLC) {
            desc[i].points = 0;
            desc[i].isBlank = true;
            blankCount--;
            descMap[index]--;
            i--;
        }
    }

    return true;
}
