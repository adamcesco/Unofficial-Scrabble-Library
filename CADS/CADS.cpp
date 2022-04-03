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
    if(data != nullptr){
        for (int i = 0; i < 15; ++i) {
            delete[] data[i];
        }
        delete[] data;
    }
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

    if(data != nullptr){
        for (int i = 0; i < 15; ++i) {
            delete[] data[i];
        }
        delete[] data;
    }
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

CADS &CADS::clear_all() {
    if(data == nullptr) {
        return *this;
    }
    for (int i = 0; i < 15; ++i) {
        for (int j = 1; j < 27; ++j) {
            data[i][j].clear();
        }
    }

    return *this;
}

vector<AnchoredString> &CADS::at_with(int x, unsigned char toFind) {
    if(data == nullptr)
        throw invalid_argument("Error in vector<AnchoredString> at_with(int, int, char) | CADS is not initialized");
    if(x > 14 || !isalpha(toFind))
        throw invalid_argument("Error in vector<AnchoredString> at_with(int, int, char) | Invalid parameter value.\nX: " + to_string(x) + "\nChar: " + char(toFind));
    return data[x][toFind & 31];
}
