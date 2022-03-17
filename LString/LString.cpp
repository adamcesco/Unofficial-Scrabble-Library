#include "LString.h"

LString::LString(const LString& toCpy){
    eleCount = toCpy.eleCount;

    for (int i = 0; i < eleCount; ++i)
        data[i] = toCpy.data[i];
}

LString& LString::operator=(const LString& toAssign){
    if(this == &toAssign)
        return *this;

    eleCount = toAssign.eleCount;

    for (int i = 0; i < eleCount; ++i)
        data[i] = toAssign.data[i];

    return *this;
}

Letter& LString::operator[](int subscript){
    if(subscript < 0 || subscript >= eleCount)
        throw invalid_argument("invalid subscript for LString::operator[](int) | subscript parameter is invalid");
    return data[subscript];
}

Letter LString::read_at(int subscript) const{
    if(subscript < 0 || subscript >= eleCount)
        throw invalid_argument("invalid subscript for LString::read_at(int) | subscript parameter is invalid");
    return data[subscript];
}

Letter LString::read_back() const{
    if(eleCount < 1)
        throw invalid_argument("invalid call for LString::back() | eleCount parameter is less than 1");
    return data[eleCount - 1];
}

Letter& LString::back(){
    if(eleCount < 1)
        throw invalid_argument("invalid call for LString::back() | eleCount parameter is less than 1");
    return data[eleCount - 1];
}

LString& LString::push_back(Letter pssd){
    if(eleCount > 44)
        throw invalid_argument("invalid call for LString& LString::push_back(Letter) | Max capacity of LString has been reached");
    data[eleCount] = pssd;
    eleCount++;
    return *this;
}

LString& LString::pop_back(){
    if(eleCount == 0)
        throw invalid_argument("invalid subscript for LString::pop_front(int) | LString::length is 0");
    eleCount--;
    return *this;
}

LString& LString::erase_at(int subscript){
    if(subscript < 0 || subscript >= eleCount)
        throw invalid_argument("invalid subscript for LString::erase_at()");

    for (int i = (subscript + 1); i < eleCount; ++i) {
        data[i - 1] = data[i];
    }
    eleCount--;

    return *this;
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
        if(dsv1.data[i].LData != data[i].LData)
            return false;
    }

    return true;
}

LString &LString::operator=(const string &toAssign) {
    int passedSize = toAssign.size();
    eleCount = (passedSize < 46) ? passedSize : throw invalid_argument("invalid subscript for LString &LString::operator=(const string &toAssign) | Passed string is of a length that is larger than the max capacity for an LString");;

    for (int i = 0; i < eleCount; ++i) {
        data[i] = toAssign[i];
    }

    return *this;
}

LString& LString::operator+=(const Letter &pssd){
    if(eleCount > 44)
        throw invalid_argument("invalid call for LString& LString::operator+=(const Letter &) | Max capacity of LString has been reached");
    data[eleCount] = pssd;
    eleCount++;
    return *this;
}

LString& LString::operator+=(char pssd) {
    if(eleCount > 44)
        throw invalid_argument("invalid call for LString& LString::operator+=(char) | Max capacity of LString has been reached");
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
    if(eleCount > 44)
        throw invalid_argument("invalid call for LString &LString::push_back(char) | Max capacity of LString has been reached");
    data[eleCount] = pssd;
    eleCount++;
    return *this;
}

string LString::to_string() const{
    string temp;
    for(int i = 0; i < eleCount; i++)
        temp += data[i].LData;
    return temp;
}

LString::LString(const string &toCpy) {
    eleCount = toCpy.length();
    for (int i = 0; i < eleCount; ++i)
        data[i] = toCpy[i];
}

LString &LString::operator=(char* toAssign) {
    int passedSize = strlen(toAssign);
    eleCount = (passedSize < 46) ? passedSize : throw invalid_argument("invalid subscript for LString &LString::operator=(char* toAssign) | Passed string is of a length that is larger than the max capacity for an LString");;

    for (int i = 0; i < eleCount; ++i)
        data[i] = toAssign[i];

    return *this;
}

bool LString::row_is_descendent_of(const string& hand, const LString& row, LString& word) {
    if(eleCount == 0)
        return false;

    int sumMap[123];
    int letterCount[123];
    int blankCount = 0;
    for (int i = 0; i < 123; ++i) {
        sumMap[i] = 0;
        letterCount[i] = 0;
    }

    for (int i = 0; i < hand.length(); ++i) {
        if(hand[i] == '?')
            blankCount++;
        sumMap[abs(toupper(hand[i]))]++;
    }
    for (int i = word[0].x; i < word.length() + word[0].x; ++i)
        sumMap[abs(toupper(row.read_at(i).LData))]++;

    for (int i = word[0].x; i < word.length() + word[0].x; ++i)
        letterCount[abs(toupper(data[i].LData))]++;

    sumMap[32] = 0;
    letterCount[32] = 0;

    for (int i = word[0].x; i < word.length() + word[0].x; ++i){
        char curChar = abs(toupper(data[i].LData));
        if(sumMap[curChar] < letterCount[curChar] && blankCount == 0)
            return false;
        else if (sumMap[curChar] < letterCount[curChar]) {
            word[i - word[0].x].val = 0;
            word[i - word[0].x].isBlank = true;
            blankCount--;
            letterCount[curChar]--;
            i--;
        }
    }

    return true;
}

LString::LString(char* toCpy) {
    eleCount = strlen(toCpy);
    for (int i = 0; i < eleCount; ++i)
        data[i] = toCpy[i];
}

int LString::get_letter_points() const{
    int sum = 0;
    for (int i = 0; i < eleCount; ++i)
        sum += data[i].val;
    return sum;
}

int LString::get_letter_points(string passed) {
    int sum = 0;
    for(char it : passed)
        sum += legend[(it & 31) - 1];
    return sum;
}

LString& LString::set_x_vals_to_subscripts() {
    for (int i = 0; i < eleCount; ++i)
        data[i].x = i;

    return *this;
}

LString &LString::add_to_x_vals(int passed) {
    for (int i = 0; i < eleCount; ++i)
        data[i].x += passed;

    return *this;
}

LString &LString::set_x_vals_equal_to(int passed) {
    for (int i = 0; i < eleCount; ++i)
        data[i].x = passed;

    return *this;
}

bool LString::contains_flag(int passed) const{
    for (int i = 0; i < eleCount; ++i) {
        if(data[i].flag == passed)
            return true;
    }
    return false;
}

vector<LString> LString::fragments() const{
    vector<LString> fragments;
    LString curFragment;
    for (int i = 0; i < eleCount; ++i) {
        if(data[i].LData != ' ') {
            curFragment += data[i];
        }
        else if (curFragment.eleCount != 0){
            fragments.push_back(curFragment);
            curFragment.clear();
        }
    }
    if (curFragment.eleCount != 0)
        fragments.push_back(curFragment);

    return fragments;
}

LString LString::operator+(const Letter& toAppend) const {
    LString temp(*this);
    if(temp.eleCount > 44)
        throw invalid_argument("invalid call for LString LString::operator+(const Letter& toAppend) | Max capacity of LString has been reached");
    temp.data[temp.eleCount] = toAppend;
    temp.eleCount++;
    return temp;
}

LString LString::operator+(char toAppend) const {
    LString temp(*this);
    if(temp.eleCount > 44)
        throw invalid_argument("invalid call for LString LString::operator+(char toAppend) | Max capacity of LString has been reached");
    temp.data[temp.eleCount] = toAppend;
    temp.eleCount++;
    return temp;
}

LString &LString::set_y_vals_equal_to(int passed) {
    for (int i = 0; i < eleCount; ++i)
        data[i].y = passed;

    return *this;
}

bool LString::is_all_whitespace() const {
    for (int i = 0; i < eleCount; ++i) {
        if(isalpha(data[i].LData))
            return false;
    }
    return true;
}

bool LString::is_descendent_of(const string& hand) {
    if(eleCount == 0)
        return false;

    int handMap[123];
    int letterCount[123];
    int blankCount = 0;
    for (int i = 0; i < 123; ++i)
        handMap[i] = letterCount[i] = 0;

    for (char it : hand) {
        if(it == '?')
            blankCount++;
        handMap[abs(toupper(it))]++;
    }

    for (int i = 0; i < eleCount; ++i)
        letterCount[abs(toupper(data[i].LData))]++;

    handMap[32] = 0;
    letterCount[32] = 0;

    for (int i = 0; i < eleCount; ++i) {
        int index = abs(toupper(data[i].LData));
        if(handMap[index] < letterCount[index] && blankCount == 0)
            return false;
        else if (handMap[index] < letterCount[index]) {
            data[i].val = 0;
            data[i].isBlank = true;
            blankCount--;
            letterCount[index]--;
            i--;
        }
    }

    return true;
}
