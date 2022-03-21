#include "CADS.h"

CADS::CADS() {
    data = new vector<AnchoredString>*[15];
    for (int i = 0; i < 15; ++i) {
        data[i] = new vector<AnchoredString>[27];
    }

    ifstream englishWords;
    englishWords.open("../Data/scrabble_word_list.txt");
    if(!englishWords.is_open())
        throw invalid_argument("Could not open ../Data/scrabble_word_list.txt");

    string curWord;
    int count = 0;
    while(englishWords.good()){
        getline(englishWords, curWord);
        count++;

        while(isspace(curWord.back()))
            curWord.pop_back();

        for (int j = 0; j < 15; ++j) {
            if(j + curWord.length() > 15)
                break;
            for (int k = 0; k < curWord.length(); ++k) {
                int index = int(abs(curWord[k]) & 31);
                data[j + k][index].emplace_back(AnchoredString(curWord, k));
            }
        }
    }
    cout << "CADS:: " << count << " words read from ../Data/scrabble_word_list.txt" << endl;

    englishWords.close();
}

CADS::CADS(string filePath){
    data = new vector<AnchoredString>*[15];
    for (int i = 0; i < 15; ++i) {
        data[i] = new vector<AnchoredString>[27];
    }

    ifstream englishWords;
    englishWords.open(filePath);
    if(!englishWords.is_open())
        throw invalid_argument("Could not open " + filePath);

    string curWord;
    int count = 0;
    while(englishWords.good()){
        getline(englishWords, curWord);
        count++;

        while(isspace(curWord.back()))
            curWord.pop_back();

        for (int j = 0; j < 15; ++j) {
            if(j + curWord.length() > 15)
                break;
            for (int k = 0; k < curWord.length(); ++k) {
                int index = int(abs(curWord[k]) & 31);
                data[j + k][index].emplace_back(AnchoredString(curWord, k));
            }
        }
    }
    cout << "CADS:: " << count << " words read from " <<  filePath << endl;

    englishWords.close();
}

CADS::~CADS() {
    for (int i = 0; i < 15; ++i)
        delete[] data[i];
    delete[] data;
}
