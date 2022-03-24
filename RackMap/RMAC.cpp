#include "RMAC.h"

RMAC::RMAC(const string& pRack) {
    rack = pRack;
    ifstream wordCorpus;
    wordCorpus.open("../Data/scrabble_word_list.txt");
    if(!wordCorpus.is_open())
        throw invalid_argument("Could not open ../Data/scrabble_word_list.txt");

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
        if(TString::is_descendent_of(toPush, rack))
            data.emplace_back(toPush);
    }
    wordCorpus.close();
    cout << "RMAC::RMAC():: " << count << " rack-information sets read from ../Data/RackDatabase.csv" << endl;
}