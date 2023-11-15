#ifndef SCRABBLELIBRARY_UTILS_SCRABBLESTRING_SCRABBLESTRING_H
#define SCRABBLELIBRARY_UTILS_SCRABBLESTRING_SCRABBLESTRING_H

#include <cstdint>
#include <optional>
#include <string>
#include <vector>

#include "Tile.h"

namespace scl {
class ScrabbleString {
   public:
    // constructors

    ScrabbleString() = default;

    ScrabbleString(std::string);

    ScrabbleString(const char *const, std::optional<int8_t>);

    // operator overloads

    ScrabbleString &operator=(const char *const);

    virtual bool operator==(const ScrabbleString &) const;

    bool operator==(const std::string &) const;

    bool operator==(const char *const) const;

    ScrabbleString &operator+=(const Tile &);

    ScrabbleString &operator+=(char);

    // accessors and mutators

    Tile &operator[](int8_t);

    Tile at(int8_t) const;

    inline int8_t length() const { return this->len_; }

    inline int8_t size() const { return this->len_; }

    Tile &back();

    // member functions

    inline void Clear() { this->len_ = 0; }

    inline bool IsEmpty() const { return this->len_ == 0; }

    virtual std::string ToString() const;

    bool RowIsDescendentOf(const std::string &, const ScrabbleString &, ScrabbleString &);

    bool IsDescendentOf(const std::string &);

    int16_t GetLetterPoints() const;

    static int16_t GetLetterPoints(std::string);

    ScrabbleString &SetXValsToSubscripts();

    ScrabbleString &AddToXVals(int16_t);

    ScrabbleString &SetXValsEqualTo(int16_t);

    ScrabbleString &SetYValsEqualTo(int16_t);

    bool ContainsFlag(int16_t) const;

    std::vector<ScrabbleString> Fragments() const;

    std::vector<std::string> StringFragments() const;

    ScrabbleString &ToVerticalFormat();

    ScrabbleString &ToHorizontalFormat();

    ScrabbleString ReturnVerticalFormat() const;

    ScrabbleString ReturnHorizontalFormat() const;

   private:
    Tile data_[20];
    int8_t len_ = 0;
};
}  // namespace scl
#endif  // SCRABBLELIBRARY_UTILS_SCRABBLESTRING_SCRABBLESTRING_H
