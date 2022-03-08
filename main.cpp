#define CATCH_CONFIG_RUNNER
#include "HorizontalScrabbleVectorizer/HorizontalScrabbleVectorizer.h"
#include "VerticalScrabbleVectorizer/VerticalScrabbleVectorizer.h"
#include "Test_Folder/catch.hpp"

//TODO: erase premium tile values when word is placed into the board

int main(int argc, char* argv[]){
    if(argc == 1) {
        Catch::Session().run();
        return 0;
    }

    string hand = "PJIAMD?";    //blank letter should be marked as a '?'

    HorizontalScrabbleVectorizer hReader(hand);
    hReader.build_board(argv[1]);
    hReader.validate_board();
    hReader.print_formatted_board();
    hReader.clear_wordSets();

    VerticalScrabbleVectorizer vReader(hand);
    vReader.build_board(argv[1]);
    vReader.validate_board();
    vReader.print_formatted_board();
    vReader.clear_wordSets();

//    vReader.search_for_intersecting_words();
//    vReader.search_for_tangential_words();
    vReader.search_for_all_words();

//    hReader.search_for_intersecting_words();
//    hReader.search_for_tangential_words();
    hReader.search_for_all_words();

    vector<int> duoVals = hReader.find_points_of_word("DUO");
    cout << "DUO - " << duoVals[0] << endl;

    vReader.validate_words();
    hReader.validate_words();

    vReader.update_best_word();
    hReader.update_best_word();
    int vPoints = vReader.points_of_best_word();
    LString bestVWord = vReader.get_best_word();
    int hPoints = hReader.points_of_best_word();
    LString bestHWord = hReader.get_best_word();

    //printing the information of the best word for the board to the console
    cout << "Best Horizontal Word: " << bestHWord.to_string() << endl;
    cout << "\tPoints: " << hPoints << endl;
    cout << "Best Vertical Word: " << bestVWord.to_string() << endl;
    cout << "\tPoints: " << vPoints << endl;

    cout << "Best word for the board: ";
    if (hPoints > vPoints) {
        cout << bestHWord.to_string() << endl;
        cout << "\tPoints: " << hPoints << endl;
        cout << "\thorizontal" << endl;
    } else {
        cout << bestVWord.to_string() << endl;
        cout << "\tPoints: " << vPoints << endl;
        cout << "\tvertical" << endl;
    }

    //printing to board
    vReader.place_into_board(bestVWord);
    hReader.place_into_board(bestHWord);
    hReader.print_formatted_board();
    vReader.print_formatted_board();

    return 0;
}

/*

const int map[26] = {101, 302, 303, 204, 105, 406, 207, 408, 109, 810, 511, 112, 313, 114, 115, 316, 1017, 118, 119, 120, 121, 422, 423, 824, 425, 1026};

//                       1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16  17 18 19 20 21 22 23 24 25  26
//                       a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p,  q, r, s, t, u, v, w, x, y,  z
const int legend[26] = { 1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10};

 * */