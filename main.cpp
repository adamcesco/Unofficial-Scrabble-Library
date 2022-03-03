#include "BoardReader/BoardReader.h"

//TODO: shorten your list of official scrabble-english words
//TODO: do not read english words everytime

int main(){
    string hand = "KLEIDAO";

    BoardReader reader(hand);
    reader.build_board();
    reader.print_board();
    reader.filter_scrabble_words_by_hand();

    for (int i = 0; i < 3; ++i) {
        //TODO: fix error in return with board, because it has an error everytime it is called when the method type is not synced with the reader type
        reader.to_vertical_reader();
        reader.search_board_for_words();
        reader.check_vir_words_perpendicular();
        reader.update_best_vir_word();

        reader.to_horizontal_reader();
        reader.search_board_for_words();
        reader.check_hor_words_perpendicular();
        reader.update_best_hor_word();

        int vPoints = reader.points_of_best_vir_word();
        LString bestVWord = reader.get_best_vir_word();
        int hPoints = reader.points_of_best_hor_word();
        LString bestHWord = reader.get_best_hor_word();

        //printed the information of the best word for the board to the console

        cout << "Best Horizontal Word: " << bestHWord.to_string() << endl;
        cout << "\tPoints: " << hPoints << endl;
        cout << "Best Vertical Word: " << bestVWord.to_string() << endl;
        cout << "\tPoints: " << vPoints << endl;

        cout << "Best word for the board: ";
        if (hPoints > vPoints) {
            reader.place_into_board(bestHWord, HORIZONTAL);
            cout << bestHWord.to_string() << endl;
            cout << "\tPoints: " << hPoints << endl;
            cout << "\thorizontal" << endl;
        } else {
            reader.place_into_board(bestVWord, VERTICAL);
            cout << bestVWord.to_string() << endl;
            cout << "\tPoints: " << vPoints << endl;
            cout << "\tvertical" << endl;
        }
        reader.print_board();
        reader.prime_for_different_mode();
    }

    return 0;
}

/*

const int map[26] = {101, 302, 303, 204, 105, 406, 207, 408, 109, 810, 511, 112, 313, 114, 115, 316, 1017, 118, 119, 120, 121, 422, 423, 824, 425, 1026};

//                       1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16  17 18 19 20 21 22 23 24 25  26
//                       a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p,  q, r, s, t, u, v, w, x, y,  z
const int legend[26] = { 1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10};

 * */