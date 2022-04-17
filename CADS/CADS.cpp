#include "CADS.h"

ssl::CADS::CADS(const char* filePath){
    data = new std::vector<AnchoredString>*[15];
    for (int i = 0; i < 15; ++i) {
        data[i] = new std::vector<AnchoredString>[27];
    }

    std::ifstream englishWords;
    englishWords.open(filePath);
    if(!englishWords.is_open())
        throw std::invalid_argument("Could not open file path passed to ssl::CADS::ssl::CADS(const char* filePath)");

    std::string curWord;
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
    std::cout << "ssl::CADS:: " << count << " words read from " <<  filePath << std::endl;

    englishWords.close();
}

ssl::CADS::~CADS() {
    if(data != nullptr){
        for (int i = 0; i < 15; ++i) {
            delete[] data[i];
        }
        delete[] data;
    }
}

ssl::CADS::CADS(const ssl::CADS & toCpy) {
    data = new std::vector<AnchoredString>*[15];
    for (int i = 0; i < 15; ++i) {
        data[i] = new std::vector<AnchoredString>[27];
    }

    for (int i = 0; i < 15; ++i) {
        for (int j = 1; j < 27; ++j) {
            data[i][j] = toCpy.data[i][j];
        }
    }
}

ssl::CADS &ssl::CADS::operator=(const ssl::CADS & toAssign) {
    if(this == &toAssign)
        return *this;

    if(data != nullptr){
        for (int i = 0; i < 15; ++i) {
            delete[] data[i];
        }
        delete[] data;
    }
    data = new std::vector<AnchoredString>*[15];
    for (int i = 0; i < 15; ++i) {
        data[i] = new std::vector<AnchoredString>[27];
    }

    for (int i = 0; i < 15; ++i) {
        for (int j = 1; j < 27; ++j) {
            data[i][j] = toAssign.data[i][j];
        }
    }

    return *this;
}

ssl::CADS &ssl::CADS::clear_all() {
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

std::vector<ssl::AnchoredString> &ssl::CADS::at_with(int x, unsigned char toFind) {
    if(data == nullptr)
        throw std::invalid_argument("Error in vector<AnchoredString> at_with(int, int, char) | ssl::CADS is not initialized");
    if(x > 14 || !isalpha(toFind))
        throw std::invalid_argument("Error in vector<AnchoredString> at_with(int, int, char) | Invalid parameter value.\nX: " + std::to_string(x) + "\nChar: " + char(toFind));
    return data[x][toFind & 31];
}
