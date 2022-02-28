



#include "LString.h"

#ifndef INC_21F_PA02_DSVECTOR_H
#define INC_21F_PA02_DSVECTOR_H

LString::LString(int size) {
    data = new Letter[size];
    eleCount = 0;
    maxCap = size;
}

void LString::increaseMaxCapacity(){
    maxCap *= 2;
    Letter* dataTemp = new Letter[maxCap];
    for (int i = 0; i < eleCount; ++i) {
        dataTemp[i] = data[i];
    }
    delete[] data;
    data = dataTemp;
}

LString::LString(){
    data = new Letter[10];
    eleCount = 0;
    maxCap = 10;
}

LString::LString(const LString& toCpy){
    eleCount = toCpy.eleCount;
    maxCap = toCpy.maxCap;
    data = new Letter[maxCap];

    for (int i = 0; i < eleCount; ++i) {
        data[i] = toCpy.data[i];
    }
}

LString& LString::operator=(const LString& toAssign){
    if(this == &toAssign)
        return *this;

    eleCount = toAssign.eleCount;
    maxCap = toAssign.maxCap;
    delete[] data;
    data = new Letter[maxCap];

    for (int i = 0; i < eleCount; ++i) {
        data[i] = toAssign.data[i];
    }

    return *this;
}

Letter& LString::operator[](int subscript){
    if(subscript < 0 || subscript >= maxCap)
        throw invalid_argument("invalid subscript for LString::operator[](int) | subscript parameter is invalid");
    return data[subscript];
}

Letter LString::read_at(int subscript) const{
    if(subscript < 0 || subscript >= maxCap)
        throw invalid_argument("invalid subscript for LString::read_at(int) | subscript parameter is invalid");
    return data[subscript];
}

int LString::size() const{
    return eleCount;
}

int LString::read_maxCap() const{
    return maxCap;
}

Letter LString::read_back() const{
    return data[eleCount - 1];
}

Letter& LString::back(){
    return data[eleCount - 1];
}

int LString::find_pos_of(Letter toFind) const{
    for (int i = 0; i < eleCount; ++i) {
        if(data[i] == toFind)
            return i;
    }

    return -1;
}

LString& LString::push_back(Letter pssd){
    if(eleCount == maxCap)
        increaseMaxCapacity();

    data[eleCount] = pssd;

    eleCount++;

    return *this;
}

LString& LString::push_front(Letter pssd){
    if(eleCount == maxCap)
        maxCap *= 2;

    Letter* dataCpy = new Letter[maxCap];
    int* flagCpy = new int[maxCap];
    dataCpy[0] = pssd;
    flagCpy[0] = -1;
    for (int i = 0; i < eleCount; ++i) {
        dataCpy[i + 1] = data[i];
    }

    delete[] data;
    data = dataCpy;
    eleCount++;

    return *this;
}

LString& LString::pop_back(){
    if(eleCount == 0)
        throw invalid_argument("invalid subscript for LString::pop_front(int) | LString::size is 0");

    if((eleCount - 1) == (maxCap / 2)) {
        maxCap /= 2;

        Letter *dataCpy = new Letter[maxCap];
        int* flagCpy = new int[maxCap];
        for (int i = 0; i < eleCount - 1; ++i) {
            dataCpy[i] = data[i];
        }

        delete[] data;
        data = dataCpy;
    }

    eleCount--;

    return *this;
}

LString& LString::pop_front(){
    if(eleCount == 0)
        throw invalid_argument("invalid subscript for LString::pop_front(int) | LString::size is 0");

    if((eleCount - 1) == (maxCap / 2))
        maxCap /= 2;

    Letter* dataCpy = new Letter[maxCap];
    int* flagCpy = new int[maxCap];

    for (int i = 1; i < eleCount; ++i) {
        dataCpy[i - 1] = data[i];
    }

    delete[] data;
    data = dataCpy;
    eleCount--;

    return *this;
}

LString& LString::clear(){
    eleCount = 0;
    maxCap = 10;
    delete[] data;
    data = new Letter[10];
    return *this;
}

LString& LString::erase_at(int subscript){
    if(subscript > (eleCount - 1) || subscript < 0)
        throw invalid_argument("invalid subscript for LString::erase_at()");

    if((eleCount - 1) == (maxCap / 2))
        maxCap /= 2;

    Letter* dataCpy = new Letter[maxCap];
    int* flagCpy = new int[maxCap];

    for (int i = 0; i < subscript; ++i) {
        dataCpy[i] = data[i];
    }
    for (int i = (subscript + 1); i < eleCount; ++i) {
        dataCpy[i - 1] = data[i];
    }

    delete[] data;
    data = dataCpy;
    eleCount--;

    return *this;
}

bool LString::is_empty() const{
    if(eleCount > 0)
        return false;
    return true;
}

bool LString::contains(Letter toFind) const{
    for (int i = 0; i < eleCount; ++i) {
        if(data[i] == toFind)
            return true;
    }

    return false;
}

bool LString::operator==(const LString& dsv1) const{
    if(eleCount != dsv1.eleCount)
        return false;

    for (int i = 0; i < eleCount; i++) {
        if(!(dsv1.data[i].LData == data[i].LData))
            return false;
    }

    return true;
}

LString &LString::operator=(const string &toAssign) {
    eleCount = toAssign.size();
    maxCap = eleCount * 2;
    delete[] data;
    data = new Letter[maxCap];

    for (int i = 0; i < eleCount; ++i) {
        data[i] = toAssign[i];
    }

    return *this;
}

LString& LString::operator+=(const Letter &pssd){
    if(eleCount == maxCap)
        increaseMaxCapacity();

    data[eleCount] = pssd;

    eleCount++;

    return *this;
}

LString& LString::operator+=(char pssd) {
    if(eleCount == maxCap)
        increaseMaxCapacity();

    data[eleCount] = pssd;

    eleCount++;

    return *this;
}

bool LString::contains(char toFind) const {
    for (int i = 0; i < eleCount; ++i) {
        if(data[i] == toFind)
            return true;
    }

    return false;
}

LString &LString::push_back(char pssd) {
    if(eleCount == maxCap)
        increaseMaxCapacity();

    data[eleCount] = pssd;

    eleCount++;

    return *this;
}

LString &LString::push_front(char pssd) {
    if(eleCount == maxCap)
        maxCap *= 2;

    Letter* dataCpy = new Letter[maxCap];
    int* flagCpy = new int[maxCap];
    dataCpy[0] = pssd;
    flagCpy[0] = -1;
    for (int i = 0; i < eleCount; ++i) {
        dataCpy[i + 1] = data[i];
    }

    delete[] data;
    data = dataCpy;
    eleCount++;

    return *this;
}

string LString::to_string() {
    string temp;
    for(const auto& it : *this){
        temp += it.LData;
    }
    return temp;
}

bool LString::contains(string pssd) const {
    LString passed = pssd;
    int pssedlength = passed.size();
    int thisLen = this->eleCount;
    if (pssedlength > thisLen)  //checks to see if passed length is greater than this->data, if so then it returns false
        return false;
    else if (pssedlength == 0 && thisLen == 0)  //if both this->data and passed have no content, then returns true
        return true;

    LString slidingWindow;
    for (int i = 0; i < passed.eleCount; i++) {    //this grows "slidingWindow" to the same length to "passed", then continuously slides the content through "slidingWindow" until "passed" is found
        slidingWindow += data[i];
        if (slidingWindow.eleCount > pssedlength)
            slidingWindow = slidingWindow.pop_front();

        if (slidingWindow == passed)    //if "passed" is found, then returns true
            return true;
    }

    return false;
}

LString::LString(const string &toCpy) {
    eleCount = toCpy.size();
    maxCap = eleCount * 2;
    data = new Letter[maxCap];

    for (int i = 0; i < eleCount; ++i) {
        data[i] = toCpy[i];
    }
}

bool LString::contains(LString passed) const {
    int pssedlength = passed.size();
    int thisLen = this->eleCount;
    if (pssedlength > thisLen)  //checks to see if passed length is greater than this->data, if so then it returns false
        return false;
    else if (pssedlength == 0 && thisLen == 0)  //if both this->data and passed have no content, then returns true
        return true;

    LString slidingWindow;
    for (int i = 0; i < passed.eleCount; i++) {    //this grows "slidingWindow" to the same length to "passed", then continuously slides the content through "slidingWindow" until "passed" is found
        slidingWindow += data[i];
        if (slidingWindow.eleCount > pssedlength)
            slidingWindow = slidingWindow.pop_front();

        if (slidingWindow == passed)    //if "passed" is found, then returns true
            return true;
    }

    return false;
}

#endif
