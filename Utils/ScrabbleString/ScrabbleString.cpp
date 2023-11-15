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
        if (dsv1.data_[i].letter != this->data_[i].letter) {
            return false;
        }
    }

    return true;
}

scl::ScrabbleString& scl::ScrabbleString::operator+=(scl::Tile pssd) {
    assert(this->len_ < 20);
    this->data_[this->len_] = std::move(pssd);
    this->len_++;
    return *this;
}

std::string scl::ScrabbleString::ToString() const {
    std::string temp(this->len_, '\0');
    for (int i = 0; i < this->len_; i++) {
        temp[i] = this->data_[i].letter;
    }
    return temp;
}

scl::ScrabbleString& scl::ScrabbleString::operator=(std::string_view toAssign) {
    assert(toAssign.size() < 21);

    this->len_ = toAssign.size();
    for (int i = 0; i < this->len_; ++i) {
        this->data_[i] = toAssign[i];
    }

    return *this;
}

scl::ScrabbleString::ScrabbleString(std::string_view toCpy) {
    assert(toCpy.size() < 21);

    this->len_ = toCpy.size();
    for (int8_t i = 0; i < this->len_; ++i) {
        this->data_[i] = toCpy[i];
    }
}

scl::ScrabbleString::ScrabbleString(const Tile* const toCpy, int8_t len) {
    assert(len < 21);

    this->len_ = len;
    for (int8_t i = 0; i < this->len_; ++i) {
        this->data_[i] = toCpy[i];
    }
}

int16_t scl::ScrabbleString::GetLetterPoints() const {
    int16_t sum = 0;
    for (int8_t i = 0; i < this->len_; ++i) {
        sum += this->data_[i].points;
    }
    return sum;
}

int16_t scl::ScrabbleString::GetLetterPoints(std::string_view passed) {
    int16_t sum = 0;
    for (char it : passed) {
        sum += kPointLegend[(it & 31) - 1];
    }
    return sum;
}

scl::ScrabbleString& scl::ScrabbleString::SetXValsToSubscripts() {
    for (int i = 0; i < this->len_; ++i) {
        this->data_[i].x = i;
    }

    return *this;
}

scl::ScrabbleString& scl::ScrabbleString::AddToXVals(int16_t passed) {
    for (int i = 0; i < this->len_; ++i) {
        this->data_[i].x += passed;
    }

    return *this;
}

scl::ScrabbleString& scl::ScrabbleString::SetXValsEqualTo(int16_t passed) {
    for (int i = 0; i < this->len_; ++i) {
        this->data_[i].x = passed;
    }

    return *this;
}

bool scl::ScrabbleString::ContainsFlag(int16_t passed) const {
    for (int i = 0; i < this->len_; ++i) {
        if (this->data_[i].flag == passed) {
            return true;
        }
    }
    return false;
}

std::vector<scl::ScrabbleString> scl::ScrabbleString::Fragments() const {
    std::vector<scl::ScrabbleString> fragments;
    scl::Tile curFragment[this->len_];
    int8_t j = 0;
    for (int i = 0; i < this->len_; ++i) {
        if (isspace(this->data_[i].letter) == false) {
            curFragment[j] = this->data_[i];
            j++;
        } else if (j > 0) {
            fragments.push_back(std::move(scl::ScrabbleString(curFragment, j)));
            j = 0;
        }
    }
    if (j > 0) {
        fragments.push_back(std::move(scl::ScrabbleString(curFragment, j)));
    }

    return fragments;
}

scl::ScrabbleString& scl::ScrabbleString::SetYValsEqualTo(int16_t passed) {
    for (int i = 0; i < this->len_; ++i) {
        this->data_[i].y = passed;
    }

    return *this;
}

std::vector<std::string> scl::ScrabbleString::StringFragments() const {
    std::vector<std::string> fragments;
    char curFragment[this->len_ + 1];
    int8_t j = 0;
    for (int i = 0; i < this->len_; ++i) {
        if (isspace(this->data_[i].letter) == false) {
            curFragment[j] = this->data_[i].letter;
            j++;
        } else if (j > 0) {
            curFragment[j] = '\0';  // not required but is good practice
            fragments.push_back(std::move(std::string(curFragment, j)));
            j = 0;
        }
    }
    if (j > 0) {
        fragments.push_back(std::move(std::string(curFragment, j)));
    }

    return fragments;
}

bool scl::ScrabbleString::operator==(std::string_view toComp) const {
    if (this->len_ != toComp.size()) {
        return false;
    }

    for (int i = 0; i < this->len_; i++) {
        if (toComp[i] != this->data_[i].letter) {
            return false;
        }
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
