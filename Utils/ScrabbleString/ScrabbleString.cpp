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

scl::Tile& scl::ScrabbleString::back() {
    assert(this->len_ > 0);
    return this->data_[this->len_ - 1];
}

bool scl::ScrabbleString::operator==(const scl::ScrabbleString& dsv1) const {
    if (this->len_ != dsv1.len_)
        return false;

    for (int i = 0; i < this->len_; i++) {
        if (dsv1.data_[i].letter != this->data_[i].letter)
            return false;
    }

    return true;
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
    size_t passedSize = strlen(toAssign);
    assert(passedSize < 21);

    this->len_ = passedSize;

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

    for (int8_t i = word[0].x; i < word.length() + word[0].x; ++i) {
        unsigned char curChar = abs(toupper(data_[i].letter));
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
    for (int8_t i = 0; i < len_; ++i)
        this->data_[i] = toCpy[i];
}

int16_t scl::ScrabbleString::GetLetterPoints() const {
    int16_t sum = 0;
    for (int8_t i = 0; i < this->len_; ++i)
        sum += this->data_[i].points;
    return sum;
}

int16_t scl::ScrabbleString::GetLetterPoints(std::string passed) {
    int16_t sum = 0;
    for (char it : passed)
        sum += kPointLegend[(it & 31) - 1];
    return sum;
}

scl::ScrabbleString& scl::ScrabbleString::SetXValsToSubscripts() {
    for (int i = 0; i < this->len_; ++i)
        this->data_[i].x = i;

    return *this;
}

scl::ScrabbleString& scl::ScrabbleString::AddToXVals(int16_t passed) {
    for (int i = 0; i < this->len_; ++i)
        this->data_[i].x += passed;

    return *this;
}

scl::ScrabbleString& scl::ScrabbleString::SetXValsEqualTo(int16_t passed) {
    for (int i = 0; i < this->len_; ++i)
        this->data_[i].x = passed;

    return *this;
}

bool scl::ScrabbleString::ContainsFlag(int16_t passed) const {
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
            curFragment += this->data_[i];
        } else if (curFragment.len_ != 0) {
            fragments.push_back(curFragment);
            curFragment.Clear();
        }
    }
    if (curFragment.len_ != 0)
        fragments.push_back(curFragment);

    return fragments;
}

scl::ScrabbleString& scl::ScrabbleString::SetYValsEqualTo(int16_t passed) {
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
        letterCount[abs(toupper(this->data_[i].letter))]++;

    for (int i = 0; i < this->len_; ++i) {
        int index = abs(toupper(this->data_[i].letter));
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
            curFragment += this->data_[i].letter;
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

bool scl::ScrabbleString::operator==(const char* const toComp) const {
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
