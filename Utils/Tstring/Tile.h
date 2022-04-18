#ifndef SCRABBLE_SOLVER_TILE_H
#define SCRABBLE_SOLVER_TILE_H

namespace scl{
    //                       1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16  17 18 19 20 21 22 23 24 25  26
    //                       a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p,  q, r, s, t, u, v, w, x, y,  z
    const int legend[26] = {1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10};

    struct Tile {
        unsigned char letter;     //think of the letter variable as the face value of the Tile object, with all other letter members being the Tile objects description
        int x, y;
        int points;
        int flag;
        bool isBlank = false;

        Tile();

        Tile(char, int, int, int);

        Tile(unsigned char, int, int, int);

        bool operator==(const Tile &invoked) const { return (invoked.letter == this->letter); }

        bool operator==(char invoked) const { return (invoked == this->letter); }

        bool operator==(unsigned char invoked) const { return (invoked == this->letter); }

        bool operator!=(const Tile &invoked) const { return (invoked.letter != this->letter); }

        bool operator!=(char invoked) const { return (invoked != this->letter); }

        Tile &operator=(const Tile &);

        Tile &operator=(char);
    };

}


#endif //SCRABBLE_SOLVER_TILE_H
