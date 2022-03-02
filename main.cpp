#include "BoardReader/BoardReader.h"

int main(){
    string hand = "regahmel";

    BoardReader horizontalReader(hand);
    horizontalReader.build_board();
    horizontalReader.to_horizontal_reader();
    horizontalReader.search_board_for_words();
    horizontalReader.check_perpendicular_compatibility();
    horizontalReader.update_best_word();

    horizontalReader.print_board();

    int hPoints = horizontalReader.get_points_of_best_word();
    cout << horizontalReader.to_string() << endl << endl;

    BoardReader verticalReader(hand);
    verticalReader.build_board();
    verticalReader.to_vertical_reader();
    verticalReader.search_board_for_words();
    verticalReader.check_perpendicular_compatibility();
    verticalReader.update_best_word();

    int vPoints = verticalReader.get_points_of_best_word();
    cout << verticalReader.to_string() << endl;

    cout << "Best word for the board: ";
    if(hPoints > vPoints){
        cout << horizontalReader.get_best_word().to_string() << endl;
        cout << "\tPoints: " << hPoints << endl;
        cout << "\tX: " << horizontalReader.get_best_x() << endl;
        cout << "\tY: " << horizontalReader.get_best_y() << endl;
    }
    else{
        cout << verticalReader.get_best_word().to_string() << endl;
        cout << "\tPoints: " << vPoints << endl;
        cout << "\tX: " << verticalReader.get_best_x() << endl;
        cout << "\tY: " << verticalReader.get_best_y() << endl;
    }

    return 0;
}

/*

const int map[26] = {101, 302, 303, 204, 105, 406, 207, 408, 109, 810, 511, 112, 313, 114, 115, 316, 1017, 118, 119, 120, 121, 422, 423, 824, 425, 1026};

//                       1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16  17 18 19 20 21 22 23 24 25  26
//                       a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p,  q, r, s, t, u, v, w, x, y,  z
const int legend[26] = { 1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10};

 * */