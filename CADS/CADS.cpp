#include "CADS.h"

CADS::CADS(const char* filePath){
    data = new vector<AnchoredString>*[15];
    for (int i = 0; i < 15; ++i) {
        data[i] = new vector<AnchoredString>[27];
    }

    ifstream englishWords;
    englishWords.open(filePath);
    if(!englishWords.is_open())
        throw invalid_argument("Could not open file path passed to CADS::CADS(const char* filePath)");

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
    for (int i = 0; i < 15; ++i) {
        delete[] data[i];
    }
    delete[] data;
}

CADS::CADS(const CADS & toCpy) {
    data = new vector<AnchoredString>*[15];
    for (int i = 0; i < 15; ++i) {
        data[i] = new vector<AnchoredString>[27];
    }

    for (int i = 0; i < 15; ++i) {
        for (int j = 1; j < 27; ++j) {
            data[i][j] = toCpy.data[i][j];
        }
    }
}

CADS &CADS::operator=(const CADS & toAssign) {
    if(this == &toAssign)
        return *this;

    for (int i = 0; i < 15; ++i) {
        delete[] data[i];
    }
    delete[] data;

    data = new vector<AnchoredString>*[15];
    for (int i = 0; i < 15; ++i) {
        data[i] = new vector<AnchoredString>[27];
    }

    for (int i = 0; i < 15; ++i) {
        for (int j = 1; j < 27; ++j) {
            data[i][j] = toAssign.data[i][j];
        }
    }

    return *this;
}
