#include "ScrabbleString.h"

#include <cassert>
#include <cstring>
#include <stdexcept>
#include <string>

scl::Tile& scl::ScrabbleString::operator[](int8_t subscript) {
    assert(subscript >= 0 && subscript < this->len_);
    return this->data_[subscript];
}

scl::Tile scl::ScrabbleString::at(int8_t subscript) const {
    assert(subscript >= 0 && subscript < this->len_);
    return this->data_[subscript];
}

scl::Tile scl::ScrabbleString::read_back() const {
    assert(this->len_ > 0);
    return this->data_[this->len_ - 1];
}

scl::Tile& scl::ScrabbleString::back() {
    assert(this->len_ > 0);
    return this->data_[this->len_ - 1];
}

bool scl::ScrabbleString::operator==(const scl::ScrabbleString& dsv1) const {
    if (this->len_ != dsv1.len_)
        return false;

    for (int i = 0; i < len_; i++) {
        if (dsv1.data_[i].letter != this->data_[i].letter)
            return false;
    }

    return true;
}

scl::ScrabbleString& scl::ScrabbleString::operator=(const std::string& toAssign) {
    int passedSize = toAssign.size();
    this->len_ = (passedSize < 21) ? passedSize : throw std::invalid_argument("invalid subscript for scl::ScrabbleWord &scl::ScrabbleWord::operator=(const string &toAssign) | Passed string is of a length that is larger than the max capacity for an scl::ScrabbleWord");

    for (int i = 0; i < this->len_; ++i) {
        this->data_[i] = toAssign[i];
    }

    return *this;
}

scl::ScrabbleString& scl::ScrabbleString::operator+=(const scl::Tile& pssd) {
    assert(this->len_ < 20);
    this->data_[this->len_] = pssd;
    this->len_++;
    return *this;
}

scl::ScrabbleString& scl::ScrabbleString::operator+=(char pssd) {
    assert(this->len_ < 20);
    this->data_[this->len_] = pssd;
    this->len_++;
    return *this;
}

std::string scl::ScrabbleString::ToString() const {
    std::string temp(this->len_, '\0');
    for (int i = 0; i < this->len_; i++)
        temp[i] = this->data_[i].letter;
    return temp;
}

scl::ScrabbleString::ScrabbleString(const std::string toCpy) {
    this->len_ = toCpy.length();
    for (int i = 0; i < this->len_; ++i)
        this->data_[i] = toCpy[i];
}

scl::ScrabbleString& scl::ScrabbleString::operator=(const char* toAssign) {
    int passedSize = strlen(toAssign);
    this->len_ = (passedSize < 21) ? passedSize : throw std::invalid_argument("invalid subscript for scl::ScrabbleWord &scl::ScrabbleWord::operator=(char* toAssign) | Passed string is of a length that is larger than the max capacity for an scl::ScrabbleWord");

    for (int i = 0; i < this->len_; ++i)
        this->data_[i] = toAssign[i];

    return *this;
}

bool scl::ScrabbleString::RowIsDescendentOf(const std::string& hand, const scl::ScrabbleString& row, scl::ScrabbleString& word) {
    if (this->len_ == 0 || word.len_ == 0)
        return false;

    int sumMap[123];
    int letterCount[123];
    int blankCount = 0;
    for (int i = 0; i < 123; ++i) {
        sumMap[i] = 0;
        letterCount[i] = 0;
    }

    for (int i = 0; i < hand.length(); ++i) {
        if (hand[i] == '?')
            blankCount++;
        sumMap[abs(toupper(hand[i]))]++;
    }
    for (int i = word[0].x; i < word.length() + word[0].x; ++i)
        sumMap[abs(toupper(row.at(i).letter))]++;

    for (int i = word[0].x; i < word.length() + word[0].x; ++i)
        letterCount[abs(toupper(this->data_[i].letter))]++;

    sumMap[32] = 0;
    letterCount[32] = 0;

    for (int i = word[0].x; i < word.length() + word[0].x; ++i) {
        char curChar = abs(toupper(data_[i].letter));
        if (sumMap[curChar] < letterCount[curChar] && blankCount == 0)
            return false;
        else if (sumMap[curChar] < letterCount[curChar]) {
            word[i - word[0].x].points = 0;
            blankCount--;
            letterCount[curChar]--;
            i--;
        }
    }

    return true;
}

scl::ScrabbleString::ScrabbleString(const char* const toCpy, std::optional<int8_t> len) {
    if (len.has_value() == false) {
        len = strlen(toCpy);
    }
    assert(len.value() < 21);

    this->len_ = len.value();
    for (int i = 0; i < len_; ++i)
        this->data_[i] = toCpy[i];
}

int scl::ScrabbleString::GetLetterPoints() const {
    int sum = 0;
    for (int i = 0; i < len_; ++i)
        sum += data_[i].points;
    return sum;
}

int scl::ScrabbleString::GetLetterPoints(std::string passed) {
    int sum = 0;
    for (char it : passed)
        sum += kPointLegend[(it & 31) - 1];
    return sum;
}

scl::ScrabbleString& scl::ScrabbleString::SetXValsToSubscripts() {
    for (int i = 0; i < this->len_; ++i)
        this->data_[i].x = i;

    return *this;
}

scl::ScrabbleString& scl::ScrabbleString::AddToXVals(int passed) {
    for (int i = 0; i < this->len_; ++i)
        this->data_[i].x += passed;

    return *this;
}

scl::ScrabbleString& scl::ScrabbleString::SetXValsEqualTo(int passed) {
    for (int i = 0; i < this->len_; ++i)
        this->data_[i].x = passed;

    return *this;
}

bool scl::ScrabbleString::ContainsFlag(int passed) const {
    for (int i = 0; i < this->len_; ++i) {
        if (this->data_[i].flag == passed)
            return true;
    }
    return false;
}

std::vector<scl::ScrabbleString> scl::ScrabbleString::Fragments() const {
    std::vector<scl::ScrabbleString> fragments;
    scl::ScrabbleString curFragment;
    for (int i = 0; i < this->len_; ++i) {
        if (this->data_[i].letter != ' ') {
            curFragment += data_[i];
        } else if (curFragment.len_ != 0) {
            fragments.push_back(curFragment);
            curFragment.Clear();
        }
    }
    if (curFragment.len_ != 0)
        fragments.push_back(curFragment);

    return fragments;
}

scl::ScrabbleString& scl::ScrabbleString::SetYValsEqualTo(int passed) {
    for (int i = 0; i < this->len_; ++i)
        this->data_[i].y = passed;

    return *this;
}

bool scl::ScrabbleString::IsDescendentOf(const std::string& rack) {
    if (this->len_ == 0 || this->len_ > rack.length())
        return false;

    int handMap[123];
    int letterCount[123];
    int blankCount = 0;
    for (int i = 0; i < 123; ++i)
        handMap[i] = letterCount[i] = 0;

    for (char it : rack) {
        if (it == '?')
            blankCount++;
        handMap[abs(toupper(it))]++;
    }

    for (int i = 0; i < this->len_; ++i)
        letterCount[abs(toupper(data_[i].letter))]++;

    for (int i = 0; i < this->len_; ++i) {
        int index = abs(toupper(data_[i].letter));
        if (handMap[index] < letterCount[index] && blankCount == 0)
            return false;
        else if (handMap[index] < letterCount[index]) {
            this->data_[i].points = 0;
            blankCount--;
            letterCount[index]--;
            i--;
        }
    }

    return true;
}

std::vector<std::string> scl::ScrabbleString::StringFragments() const {
    std::vector<std::string> fragments;
    std::string curFragment;
    for (int i = 0; i < this->len_; ++i) {
        if (this->data_[i].letter != ' ') {
            curFragment += data_[i].letter;
        } else if (curFragment.length() != 0) {
            fragments.push_back(curFragment);
            curFragment.clear();
        }
    }
    if (curFragment.length() != 0)
        fragments.push_back(curFragment);

    return fragments;
}

bool scl::ScrabbleString::operator==(const std::string& toComp) const {
    if (this->len_ != toComp.length())
        return false;

    for (int i = 0; i < this->len_; i++) {
        if (toComp[i] != this->data_[i].letter)
            return false;
    }

    return true;
}

bool scl::ScrabbleString::operator==(const char* toComp) const {
    if (this->len_ != strlen(toComp))
        return false;

    for (int i = 0; i < this->len_; i++) {
        if (toComp[i] != this->data_[i].letter)
            return false;
    }

    return true;
}

scl::ScrabbleString& scl::ScrabbleString::ToVerticalFormat() {
    for (int i = 0; i < this->len_; ++i) {
        int temp = 14 - this->data_[i].x;
        this->data_[i].x = this->data_[i].y;
        this->data_[i].y = temp;
    }
    return *this;
}

scl::ScrabbleString& scl::ScrabbleString::ToHorizontalFormat() {
    for (int i = 0; i < this->len_; ++i) {
        int temp = 14 - this->data_[i].y;
        this->data_[i].y = this->data_[i].x;
        this->data_[i].x = temp;
    }
    return *this;
}

scl::ScrabbleString scl::ScrabbleString::ReturnVerticalFormat() const {
    ScrabbleString toReturn = *this;
    for (int i = 0; i < this->len_; ++i) {
        int temp = 14 - this->data_[i].x;
        toReturn[i].x = this->data_[i].y;
        toReturn[i].y = temp;
    }
    return toReturn;
}

scl::ScrabbleString scl::ScrabbleString::ReturnHorizontalFormat() const {
    ScrabbleString toReturn = *this;
    for (int i = 0; i < this->len_; ++i) {
        int temp = 14 - this->data_[i].y;
        toReturn[i].y = this->data_[i].x;
        toReturn[i].x = temp;
    }
    return toReturn;
}
