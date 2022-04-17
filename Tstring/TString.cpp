#include "Tstring.h"

ssl::Tstring::Tstring(const ssl::Tstring& toCpy){
    eleCount = toCpy.eleCount;

    for (int i = 0; i < eleCount; ++i)
        data[i] = toCpy.data[i];
}

ssl::Tstring& ssl::Tstring::operator=(const ssl::Tstring& toAssign){
    if(this == &toAssign)
        return *this;

    eleCount = toAssign.eleCount;

    for (int i = 0; i < eleCount; ++i)
        data[i] = toAssign.data[i];

    return *this;
}

ssl::Tile& ssl::Tstring::operator[](int subscript){
    if(subscript < 0 || subscript >= eleCount)
        throw std::invalid_argument("invalid subscript for ssl::Tstring::operator[](int) | subscript parameter is invalid");
    return data[subscript];
}

ssl::Tile ssl::Tstring::read_at(int subscript) const{
    if(subscript < 0 || subscript >= eleCount)
        throw std::invalid_argument("invalid subscript for ssl::Tstring::read_at(int) | subscript parameter is invalid");
    return data[subscript];
}

ssl::Tile ssl::Tstring::read_back() const{
    if(eleCount < 1)
        throw std::invalid_argument("invalid call for ssl::Tstring::back() | eleCount parameter is less than 1");
    return data[eleCount - 1];
}

ssl::Tile& ssl::Tstring::back(){
    if(eleCount < 1)
        throw std::invalid_argument("invalid call for ssl::Tstring::back() | eleCount parameter is less than 1");
    return data[eleCount - 1];
}

ssl::Tstring& ssl::Tstring::pop_back(){
    if(eleCount == 0)
        throw std::invalid_argument("invalid subscript for ssl::Tstring::pop_front(int) | ssl::Tstring::length is 0");
    eleCount--;
    return *this;
}

ssl::Tstring& ssl::Tstring::erase_at(int subscript){
    if(subscript < 0 || subscript >= eleCount)
        throw std::invalid_argument("invalid subscript for ssl::Tstring::erase_at()");

    for (int i = (subscript + 1); i < eleCount; ++i) {
        data[i - 1] = data[i];
    }
    eleCount--;

    return *this;
}

bool ssl::Tstring::contains(ssl::Tile toFind) const{
    for (int i = 0; i < eleCount; ++i) {
        if(data[i] == toFind)
            return true;
    }

    return false;
}

bool ssl::Tstring::operator==(const ssl::Tstring& dsv1) const{
    if(eleCount != dsv1.eleCount)
        return false;

    for (int i = 0; i < eleCount; i++) {
        if(dsv1.data[i].letter != data[i].letter)
            return false;
    }

    return true;
}

ssl::Tstring &ssl::Tstring::operator=(const std::string &toAssign) {
    int passedSize = toAssign.size();
    eleCount = (passedSize < 19) ? passedSize : throw std::invalid_argument("invalid subscript for ssl::Tstring &ssl::Tstring::operator=(const string &toAssign) | Passed string is of a length that is larger than the max capacity for an ssl::Tstring");;

    for (int i = 0; i < eleCount; ++i) {
        data[i] = toAssign[i];
    }

    return *this;
}

ssl::Tstring& ssl::Tstring::operator+=(const ssl::Tile &pssd){
    if(eleCount > 18)
        throw std::invalid_argument("invalid call for ssl::Tstring& ssl::Tstring::operator+=(const ssl::Tile &) | Max capacity of ssl::Tstring has been reached");
    data[eleCount] = pssd;
    eleCount++;
    return *this;
}

ssl::Tstring& ssl::Tstring::operator+=(char pssd) {
    if(eleCount > 18)
        throw std::invalid_argument("invalid call for ssl::Tstring& ssl::Tstring::operator+=(char) | Max capacity of ssl::Tstring has been reached");
    data[eleCount] = pssd;
    eleCount++;
    return *this;
}

bool ssl::Tstring::contains(char toFind) const {
    for (int i = 0; i < eleCount; ++i) {
        if(data[i] == toFind)
            return true;
    }
    return false;
}

std::string ssl::Tstring::to_string() const{
    std::string temp;
    for(int i = 0; i < eleCount; i++)
        temp += data[i].letter;
    return temp;
}

ssl::Tstring::Tstring(const std::string toCpy) {
    eleCount = toCpy.length();
    for (int i = 0; i < eleCount; ++i)
        data[i] = toCpy[i];
}

ssl::Tstring &ssl::Tstring::operator=(const char* toAssign) {
    int passedSize = strlen(toAssign);
    eleCount = (passedSize < 19) ? passedSize : throw std::invalid_argument("invalid subscript for ssl::Tstring &ssl::Tstring::operator=(char* toAssign) | Passed string is of a length that is larger than the max capacity for an ssl::Tstring");;

    for (int i = 0; i < eleCount; ++i)
        data[i] = toAssign[i];

    return *this;
}

bool ssl::Tstring::row_is_descendent_of(const std::string& hand, const ssl::Tstring& row, ssl::Tstring& word) {
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

ssl::Tstring::Tstring(const char* toCpy) {
    eleCount = strlen(toCpy);
    for (int i = 0; i < eleCount; ++i)
        data[i] = toCpy[i];
}

int ssl::Tstring::get_letter_points() const{
    int sum = 0;
    for (int i = 0; i < eleCount; ++i)
        sum += data[i].points;
    return sum;
}

int ssl::Tstring::get_letter_points(std::string passed) {
    int sum = 0;
    for(char it : passed)
        sum += legend[(it & 31) - 1];
    return sum;
}

ssl::Tstring& ssl::Tstring::set_x_vals_to_subscripts() {
    for (int i = 0; i < eleCount; ++i)
        data[i].x = i;

    return *this;
}

ssl::Tstring &ssl::Tstring::add_to_x_vals(int passed) {
    for (int i = 0; i < eleCount; ++i)
        data[i].x += passed;

    return *this;
}

ssl::Tstring &ssl::Tstring::set_x_vals_equal_to(int passed) {
    for (int i = 0; i < eleCount; ++i)
        data[i].x = passed;

    return *this;
}

bool ssl::Tstring::contains_flag(int passed) const{
    for (int i = 0; i < eleCount; ++i) {
        if(data[i].flag == passed)
            return true;
    }
    return false;
}

std::vector<ssl::Tstring> ssl::Tstring::fragments() const{
    std::vector<ssl::Tstring> fragments;
    ssl::Tstring curFragment;
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

ssl::Tstring ssl::Tstring::operator+(const ssl::Tile& toAppend) const {
    ssl::Tstring temp(*this);
    if(temp.eleCount > 18)
        throw std::invalid_argument("invalid call for ssl::Tstring ssl::Tstring::operator+(const ssl::Tile& toAppend) | Max capacity of ssl::Tstring has been reached");
    temp.data[temp.eleCount] = toAppend;
    temp.eleCount++;
    return temp;
}

ssl::Tstring ssl::Tstring::operator+(char toAppend) const {
    ssl::Tstring temp(*this);
    if(temp.eleCount > 18)
        throw std::invalid_argument("invalid call for ssl::Tstring ssl::Tstring::operator+(char toAppend) | Max capacity of ssl::Tstring has been reached");
    temp.data[temp.eleCount] = toAppend;
    temp.eleCount++;
    return temp;
}

ssl::Tstring &ssl::Tstring::set_y_vals_equal_to(int passed) {
    for (int i = 0; i < eleCount; ++i)
        data[i].y = passed;

    return *this;
}

bool ssl::Tstring::is_all_whitespace() const {
    for (int i = 0; i < eleCount; ++i) {
        if(isalpha(data[i].letter))
            return false;
    }
    return true;
}

bool ssl::Tstring::is_descendent_of(const std::string& rack) {
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

int ssl::Tstring::find_pos_of(char toFind) const {
    for (int i = 0; i < eleCount; ++i) {
        if(data[i] == toFind)
            return i;
    }
    return -1;
}

std::vector<std::string> ssl::Tstring::string_fragments() const {
    std::vector<std::string> fragments;
    std::string curFragment;
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

bool ssl::Tstring::operator==(const std::string& toComp) const {
    if(eleCount != toComp.length())
        return false;

    for (int i = 0; i < eleCount; i++) {
        if(toComp[i] != data[i].letter)
            return false;
    }

    return true;
}

bool ssl::Tstring::operator==(const char* toComp) const {
    if(eleCount != strlen(toComp))
        return false;

    for (int i = 0; i < eleCount; i++) {
        if(toComp[i] != data[i].letter)
            return false;
    }

    return true;
}

bool ssl::Tstring::is_descendent_of(ssl::Tstring& sub, const std::string& rack) {
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
        sub[i].x = i;
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
    }

    return true;
}