#include "CADS.h"

scl::CADS::CADS(const char* filePath){
    data = new std::vector<AnchoredString>*[15];
    for (int i = 0; i < 15; ++i) {
        data[i] = new std::vector<AnchoredString>[27];
    }

    std::ifstream englishWords;
    englishWords.open(filePath);
    if(!englishWords.is_open())
        throw std::invalid_argument("Could not open file path passed to scl::CADS::scl::CADS(const char* filePath)");

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
    std::cout << "scl::CADS:: " << count << " words read from " <<  filePath << std::endl;

    englishWords.close();
}

scl::CADS::~CADS() {
    if(data != nullptr){
        for (int i = 0; i < 15; ++i) {
            delete[] data[i];
        }
        delete[] data;
    }
}

scl::CADS::CADS(const scl::CADS & toCpy) {
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

scl::CADS &scl::CADS::operator=(const scl::CADS & toAssign) {
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

scl::CADS &scl::CADS::clear_all() {
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

std::vector<scl::AnchoredString> &scl::CADS::at_with(int x, unsigned char toFind) {
    if(data == nullptr)
        throw std::invalid_argument("Error in vector<AnchoredString> at_with(int, int, char) | scl::CADS is not initialized");
    if(x > 14 || !isalpha(toFind))
        throw std::invalid_argument("Error in vector<AnchoredString> at_with(int, int, char) | Invalid parameter value.\nX: " + std::to_string(x) + "\nChar: " + char(toFind));
    return data[x][toFind & 31];
}
