#include "HorizontalBoardReader/HorizontalBoardReader.h"
#include "VerticalBoardReader/VerticalBoardReader.h"

//TODO: shorten your list of official scrabble-english words
//TODO: do not read english words everytime

int main(){
    string hand = "KLEIDAO";

    HorizontalBoardReader hReader(hand);
    hReader.build_board();
    hReader.print_board();
    hReader.filter_scrabble_words_by_hand();

    VerticalBoardReader vReader(hand);
    vReader.build_board();
    vReader.print_board();
    vReader.filter_scrabble_words_by_hand();

    for (int i = 0; i < 1; ++i) {
        //TODO: fix error in return with board, because it has an error everytime it is called when the method type is not synced with the hReader type
        vReader.search_board_for_words();
        vReader.validate_words_perpendicular();
        vReader.update_best_word();

        hReader.search_board_for_words();
        hReader.validate_words_perpendicular();
        hReader.update_best_word();

        int vPoints = vReader.points_of_best_word();
        LString bestVWord = vReader.get_best_word();
        int hPoints = hReader.points_of_best_word();
        LString bestHWord = hReader.get_best_word();

        //printed the information of the best word for the board to the console

        cout << "Best Horizontal Word: " << bestHWord.to_string() << endl;
        cout << "\tPoints: " << hPoints << endl;
        cout << "Best Vertical Word: " << bestVWord.to_string() << endl;
        cout << "\tPoints: " << vPoints << endl;

        cout << "Best word for the board: ";
        if (hPoints > vPoints) {
            hReader.place_into_board(bestHWord);
            cout << bestHWord.to_string() << endl;
            cout << "\tPoints: " << hPoints << endl;
            cout << "\thorizontal" << endl;
        } else {
            vReader.place_into_board(bestVWord);
            cout << bestVWord.to_string() << endl;
            cout << "\tPoints: " << vPoints << endl;
            cout << "\tvertical" << endl;
        }
        hReader.print_board();
        vReader.print_board();
    }

    return 0;
}

/*

const int map[26] = {101, 302, 303, 204, 105, 406, 207, 408, 109, 810, 511, 112, 313, 114, 115, 316, 1017, 118, 119, 120, 121, 422, 423, 824, 425, 1026};

//                       1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16  17 18 19 20 21 22 23 24 25  26
//                       a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p,  q, r, s, t, u, v, w, x, y,  z
const int legend[26] = { 1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10};

 * */