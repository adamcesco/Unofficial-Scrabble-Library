#include "ScrabbleDataset.h"

ScrabbleDataset::ScrabbleDataset() {
    data = new vector<string>**[15];
    for (int i = 0; i < 15; ++i) {
        data[i] = new vector<string>*[15];
        for (int j = 0; j < 15; ++j) {
            data[i][j] = new vector<string>[27];
        }
    }

    ifstream englishWords;
    englishWords.open("../Data/scrabble_word_list.txt");
    if(!englishWords.is_open())
        throw invalid_argument("Could not open ../Data/scrabble_word_list.txt");

    string curWord;
    while(englishWords.good()){
        getline(englishWords, curWord);

        if(curWord.length() > 15)
            continue;

        for (int i = 0; i < 15; ++i) {
            for (int j = 0; j < 15; ++j) {
                if(j + curWord.length() > 15)
                    break;
                for (int k = 0; k < curWord.length(); ++k) {
                    char curChar = curWord[k];
                    data[i][j + k][abs(curChar) & 31].emplace_back(curWord);
                }
            }
        }
    }
    englishWords.close();
}

ScrabbleDataset::ScrabbleDataset(string filePath){
    data = new vector<string>**[15];
    for (int i = 0; i < 15; ++i) {
        data[i] = new vector<string>*[15];
        for (int j = 0; j < 15; ++j) {
            data[i][j] = new vector<string>[27];
        }
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

        for (int i = 0; i < 15; ++i) {
            for (int j = 0; j < 15; ++j) {
                if(j + curWord.length() > 15)
                    break;
                for (int k = 0; k < curWord.length(); ++k) {
                    char curChar = curWord[k];
                    data[i][j + k][abs(curChar) & 31].emplace_back(curWord);
                }
            }
        }
    }
    englishWords.close();
}
