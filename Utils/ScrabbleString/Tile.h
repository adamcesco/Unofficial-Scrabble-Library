#ifndef SCRABBLELIBRARY_UTILS_SCRABBLESTRING_TILE_H
#define SCRABBLELIBRARY_UTILS_SCRABBLESTRING_TILE_H

#include <cstdint>

namespace scl {
//                         1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16  17 18 19 20 21 22 23 24 25  26
//                         a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p,  q, r, s, t, u, v, w, x, y,  z
const int8_t kPointLegend[26] = {1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10};

struct Tile {
    Tile();

    Tile(unsigned char, int8_t, int8_t, int8_t);

    Tile &operator=(unsigned char);

    bool inline operator==(unsigned char invoked) const { return (invoked == this->letter); }

    bool inline operator!=(unsigned char invoked) const { return (invoked != this->letter); }
    
    unsigned char letter;
    int8_t x;
    int8_t y;
    int8_t points;
    int8_t flag;
};

}  // namespace scl

#endif  // SCRABBLELIBRARY_UTILS_SCRABBLESTRING_TILE_H
