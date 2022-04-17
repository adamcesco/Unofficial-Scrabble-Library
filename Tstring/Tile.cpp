#include "Tile.h"

scl::Tile::Tile() {
    x = y = flag = -1;
    points = 0;
    letter = ' ';
    isBlank = false;
}

scl::Tile::Tile(char passed, int xp, int yp, int fl) {
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

scl::Tile &scl::Tile::operator=(const scl::Tile &invoked) {
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

scl::Tile &scl::Tile::operator=(char invoked) {
    letter = abs(invoked);
    y = x = flag = -1;
    isBlank = false;

    if(isalpha(letter))
        points = legend[(letter & 31) - 1];
    else
        points = 0;
    return *this;
}

scl::Tile::Tile(unsigned char passed, int xp, int yp, int fl) {
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
