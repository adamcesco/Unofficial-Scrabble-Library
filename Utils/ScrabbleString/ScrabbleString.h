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

    explicit ScrabbleString(std::string_view);

    explicit ScrabbleString(const Tile * const, int8_t);

    // operator overloads

    ScrabbleString &operator=(std::string_view);

    bool operator==(const ScrabbleString &) const;

    bool operator==(std::string_view) const;

    ScrabbleString &operator+=(Tile);

    // accessors and mutators

    Tile &operator[](int8_t);

    Tile at(int8_t) const;

    inline int8_t length() const { return this->len_; }

    inline int8_t size() const { return this->len_; }

    Tile &back();

    // member functions

    inline void Clear() { this->len_ = 0; }

    inline bool IsEmpty() const { return (this->len_ == 0); }

    std::string ToString() const;

    int16_t GetLetterPoints() const;

    static int16_t GetLetterPoints(std::string_view);

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
