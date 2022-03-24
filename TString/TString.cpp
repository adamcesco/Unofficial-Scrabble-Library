#include "TString.h"

TString::TString(const TString& toCpy){
    eleCount = toCpy.eleCount;

    for (int i = 0; i < eleCount; ++i)
        data[i] = toCpy.data[i];
}

TString& TString::operator=(const TString& toAssign){
    if(this == &toAssign)
        return *this;

    eleCount = toAssign.eleCount;

    for (int i = 0; i < eleCount; ++i)
        data[i] = toAssign.data[i];

    return *this;
}

Tile& TString::operator[](int subscript){
    if(subscript < 0 || subscript >= eleCount)
        throw invalid_argument("invalid subscript for TString::operator[](int) | subscript parameter is invalid");
    return data[subscript];
}

Tile TString::read_at(int subscript) const{
    if(subscript < 0 || subscript >= eleCount)
        throw invalid_argument("invalid subscript for TString::read_at(int) | subscript parameter is invalid");
    return data[subscript];
}

Tile TString::read_back() const{
    if(eleCount < 1)
        throw invalid_argument("invalid call for TString::back() | eleCount parameter is less than 1");
    return data[eleCount - 1];
}

Tile& TString::back(){
    if(eleCount < 1)
        throw invalid_argument("invalid call for TString::back() | eleCount parameter is less than 1");
    return data[eleCount - 1];
}

TString& TString::pop_back(){
    if(eleCount == 0)
        throw invalid_argument("invalid subscript for TString::pop_front(int) | TString::length is 0");
    eleCount--;
    return *this;
}

TString& TString::erase_at(int subscript){
    if(subscript < 0 || subscript >= eleCount)
        throw invalid_argument("invalid subscript for TString::erase_at()");

    for (int i = (subscript + 1); i < eleCount; ++i) {
        data[i - 1] = data[i];
    }
    eleCount--;

    return *this;
}

bool TString::contains(Tile toFind) const{
    for (int i = 0; i < eleCount; ++i) {
        if(data[i] == toFind)
            return true;
    }

    return false;
}

bool TString::operator==(const TString& dsv1) const{
    if(eleCount != dsv1.eleCount)
        return false;

    for (int i = 0; i < eleCount; i++) {
        if(dsv1.data[i].letter != data[i].letter)
            return false;
    }

    return true;
}

TString &TString::operator=(const string &toAssign) {
    int passedSize = toAssign.size();
    eleCount = (passedSize < 46) ? passedSize : throw invalid_argument("invalid subscript for TString &TString::operator=(const string &toAssign) | Passed string is of a length that is larger than the max capacity for an TString");;

    for (int i = 0; i < eleCount; ++i) {
        data[i] = toAssign[i];
    }

    return *this;
}

TString& TString::operator+=(const Tile &pssd){
    if(eleCount > 44)
        throw invalid_argument("invalid call for TString& TString::operator+=(const Tile &) | Max capacity of TString has been reached");
    data[eleCount] = pssd;
    eleCount++;
    return *this;
}

TString& TString::operator+=(char pssd) {
    if(eleCount > 44)
        throw invalid_argument("invalid call for TString& TString::operator+=(char) | Max capacity of TString has been reached");
    data[eleCount] = pssd;
    eleCount++;
    return *this;
}

bool TString::contains(char toFind) const {
    for (int i = 0; i < eleCount; ++i) {
        if(data[i] == toFind)
            return true;
    }
    return false;
}

string TString::to_string() const{
    string temp;
    for(int i = 0; i < eleCount; i++)
        temp += data[i].letter;
    return temp;
}

TString::TString(const string toCpy) {
    eleCount = toCpy.length();
    for (int i = 0; i < eleCount; ++i)
        data[i] = toCpy[i];
}

TString &TString::operator=(const char* toAssign) {
    int passedSize = strlen(toAssign);
    eleCount = (passedSize < 46) ? passedSize : throw invalid_argument("invalid subscript for TString &TString::operator=(char* toAssign) | Passed string is of a length that is larger than the max capacity for an TString");;

    for (int i = 0; i < eleCount; ++i)
        data[i] = toAssign[i];

    return *this;
}

bool TString::row_is_descendent_of(const string& hand, const TString& row, TString& word) {
    if(eleCount == 0 || word.eleCount == 0)
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
        sumMap[abs(toupper(row.read_at(i).letter))]++;

    for (int i = word[0].x; i < word.length() + word[0].x; ++i)
        letterCount[abs(toupper(data[i].letter))]++;

    sumMap[32] = 0;
    letterCount[32] = 0;

    for (int i = word[0].x; i < word.length() + word[0].x; ++i){
        char curChar = abs(toupper(data[i].letter));
        if(sumMap[curChar] < letterCount[curChar] && blankCount == 0)
            return false;
        else if (sumMap[curChar] < letterCount[curChar]) {
            word[i - word[0].x].points = 0;
            word[i - word[0].x].isBlank = true;
            blankCount--;
            letterCount[curChar]--;
            i--;
        }
    }

    return true;
}

TString::TString(const char* toCpy) {
    eleCount = strlen(toCpy);
    for (int i = 0; i < eleCount; ++i)
        data[i] = toCpy[i];
}

int TString::get_letter_points() const{
    int sum = 0;
    for (int i = 0; i < eleCount; ++i)
        sum += data[i].points;
    return sum;
}

int TString::get_letter_points(string passed) {
    int sum = 0;
    for(char it : passed)
        sum += legend[(it & 31) - 1];
    return sum;
}

TString& TString::set_x_vals_to_subscripts() {
    for (int i = 0; i < eleCount; ++i)
        data[i].x = i;

    return *this;
}

TString &TString::add_to_x_vals(int passed) {
    for (int i = 0; i < eleCount; ++i)
        data[i].x += passed;

    return *this;
}

TString &TString::set_x_vals_equal_to(int passed) {
    for (int i = 0; i < eleCount; ++i)
        data[i].x = passed;

    return *this;
}

bool TString::contains_flag(int passed) const{
    for (int i = 0; i < eleCount; ++i) {
        if(data[i].flag == passed)
            return true;
    }
    return false;
}

vector<TString> TString::fragments() const{
    vector<TString> fragments;
    TString curFragment;
    for (int i = 0; i < eleCount; ++i) {
        if(data[i].letter != ' ') {
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

TString TString::operator+(const Tile& toAppend) const {
    TString temp(*this);
    if(temp.eleCount > 44)
        throw invalid_argument("invalid call for TString TString::operator+(const Tile& toAppend) | Max capacity of TString has been reached");
    temp.data[temp.eleCount] = toAppend;
    temp.eleCount++;
    return temp;
}

TString TString::operator+(char toAppend) const {
    TString temp(*this);
    if(temp.eleCount > 44)
        throw invalid_argument("invalid call for TString TString::operator+(char toAppend) | Max capacity of TString has been reached");
    temp.data[temp.eleCount] = toAppend;
    temp.eleCount++;
    return temp;
}

TString &TString::set_y_vals_equal_to(int passed) {
    for (int i = 0; i < eleCount; ++i)
        data[i].y = passed;

    return *this;
}

bool TString::is_all_whitespace() const {
    for (int i = 0; i < eleCount; ++i) {
        if(isalpha(data[i].letter))
            return false;
    }
    return true;
}

bool TString::is_descendent_of(const string& rack) {
    if(eleCount == 0 || eleCount > rack.length())
        return false;

    int handMap[123];
    int letterCount[123];
    int blankCount = 0;
    for (int i = 0; i < 123; ++i)
        handMap[i] = letterCount[i] = 0;

    for (char it : rack) {
        if(it == '?')
            blankCount++;
        handMap[abs(toupper(it))]++;
    }

    for (int i = 0; i < eleCount; ++i)
        letterCount[abs(toupper(data[i].letter))]++;

    for (int i = 0; i < eleCount; ++i) {
        int index = abs(toupper(data[i].letter));
        if(handMap[index] < letterCount[index] && blankCount == 0)
            return false;
        else if (handMap[index] < letterCount[index]) {
            data[i].points = 0;
            data[i].isBlank = true;
            blankCount--;
            letterCount[index]--;
            i--;
        }
    }

    return true;
}

int TString::find_pos_of(char toFind) const {
    for (int i = 0; i < eleCount; ++i) {
        if(data[i] == toFind)
            return i;
    }
    return -1;
}

vector<string> TString::string_fragments() const {
    vector<string> fragments;
    string curFragment;
    for (int i = 0; i < eleCount; ++i) {
        if(data[i].letter != ' ') {
            curFragment += data[i].letter;
        }
        else if (curFragment.length() != 0){
            fragments.push_back(curFragment);
            curFragment.clear();
        }
    }
    if (curFragment.length() != 0)
        fragments.push_back(curFragment);

    return fragments;
}

bool TString::operator==(const string& toComp) const {
    if(eleCount != toComp.length())
        return false;

    for (int i = 0; i < eleCount; i++) {
        if(toComp[i] != data[i].letter)
            return false;
    }

    return true;
}

bool TString::operator==(const char* toComp) const {
    if(eleCount != strlen(toComp))
        return false;

    for (int i = 0; i < eleCount; i++) {
        if(toComp[i] != data[i].letter)
            return false;
    }

    return true;
}

bool TString::is_descendent_of(TString& sub, const string& rack) {
    int slen = sub.eleCount;
    int rlen = rack.length();
    if(rlen == 0 || slen > rack.length())
        return false;

    int rCount[123];
    int lCount[123];
    int blankCount = 0;
    for (int i = 0; i < 123; ++i)
        rCount[i] = lCount[i] = 0;

    for (char it : rack) {
        if(it == '?')
            blankCount++;
        rCount[abs(toupper(it))]++;
    }

    for (int i = 0; i < slen; ++i)
        lCount[sub[i].letter]++;

    for (int i = 0; i < slen; ++i) {
        int index = sub[i].letter;
        bool RLC = rCount[index] < lCount[index];
        if(blankCount == 0 && RLC)
            return false;
        else if (RLC) {
            sub[i].points = 0;
            sub[i].isBlank = true;
            blankCount--;
            lCount[index]--;
            i--;
        }
        sub[i].x = i;
    }

    return true;
}