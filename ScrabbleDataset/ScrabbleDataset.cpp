#include "ScrabbleDataset.h"

ScrabbleDataset::ScrabbleDataset(string filePath){
    data = new vector<string>[1366];
    for (int i = 0; i < 1366; ++i) {
        data[i] = vector<string>();
    }

    ifstream englishWords;
    englishWords.open(filePath);
    if(!englishWords.is_open())
        throw invalid_argument("Could not open " + filePath);

    string curWord;
    while(englishWords.good()){
        getline(englishWords, curWord);

        if(curWord.length() > 15)
            continue;

        for (int i = 0; i < curWord.length(); ++i) {
            char curChar = curWord[i];
            data[(abs(curChar) * 15) + i].emplace_back(curWord);
        }
    }
    englishWords.close();
}
