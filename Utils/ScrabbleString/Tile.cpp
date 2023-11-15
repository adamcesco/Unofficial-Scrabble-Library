#include "Tile.h"

#include <fstream>

scl::Tile::Tile() {
    this->x = -1;
    this->y = -1;
    this->flag = -1;
    this->points = 0;
    this->letter = ' ';
}

scl::Tile::Tile(unsigned char passed, int16_t xp, int16_t yp, int16_t fl) {
    this->letter = toupper(passed);
    this->x = xp;
    this->y = yp;
    this->flag = fl;

    if (isalpha(letter))
        this->points = kPointLegend[(this->letter & 31) - 1];
    else
        this->points = 0;
}

scl::Tile &scl::Tile::operator=(unsigned char invoked) {
    this->letter = toupper(invoked);
    this->y = -1;
    this->x = -1;
    this->flag = -1;

    if (isalpha(letter))
        this->points = kPointLegend[(this->letter & 31) - 1];
    else
        this->points = 0;
    return *this;
}