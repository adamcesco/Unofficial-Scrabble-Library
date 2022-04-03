#include "Tile.h"

Tile::Tile() {
    x = y = flag = -1;
    points = 0;
    letter = ' ';
    isBlank = false;
}

Tile::Tile(char passed, int xp, int yp, int fl) {
    letter = abs(passed);
    x = xp;
    y = yp;
    flag = fl;
    isBlank = false;

    if(isalpha(letter))
        points = legend[(letter & 31) - 1];
    else
        points = 0;
}

Tile &Tile::operator=(const Tile &invoked) {
    if(this == &invoked)
        return *this;
    letter = invoked.letter;
    y = invoked.y;
    x = invoked.x;
    flag = invoked.flag;
    points = invoked.points;
    isBlank = invoked.isBlank;
    return *this;
}

Tile &Tile::operator=(char invoked) {
    letter = abs(invoked);
    y = x = flag = -1;
    isBlank = false;

    if(isalpha(letter))
        points = legend[(letter & 31) - 1];
    else
        points = 0;
    return *this;
}

Tile::Tile(unsigned char passed, int xp, int yp, int fl) {
    letter = passed;
    x = xp;
    y = yp;
    flag = fl;
    isBlank = false;

    if(isalpha(letter))
        points = legend[(letter & 31) - 1];
    else
        points = 0;
}
