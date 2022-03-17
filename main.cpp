#define CATCH_CONFIG_RUNNER
#include "HorizontalScrabbleVectorizer/HorizontalScrabbleVectorizer.h"
#include "VerticalScrabbleVectorizer/VerticalScrabbleVectorizer.h"
#include "Test_Folder/catch.hpp"
#include <chrono>

int main(int argc, char* argv[]){
    if(argc == 1) {
        Catch::Session().run();
        return 0;
    }

    string hand = "ADFUJEL";    //blank tiles should be marked as a '?'

    HorizontalScrabbleVectorizer hReader(hand);
    hReader.build_board(argv[1]);
    hReader.validate_board();
    hReader.console_print_formatted_board();
    hReader.clear_wordSets();

    VerticalScrabbleVectorizer vReader(hand);
    vReader.build_board(argv[1]);
    vReader.validate_board();
    vReader.console_print_formatted_board();
    vReader.clear_wordSets();

    //start timer here (timing word generation)
    auto start = chrono::high_resolution_clock::now();
    vReader.search_for_intersecting_words();
    vReader.search_for_tangential_words();

    hReader.search_for_intersecting_words();
    hReader.search_for_tangential_words();
    //end timer here
    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::seconds>(stop - start);
    cout << "Time taken by word generation: " << duration.count() << " seconds" << endl;

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
    hReader.console_print_formatted_board();
    vReader.console_print_formatted_board();

    return 0;
}

/*
                         1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16  17 18 19 20 21 22 23 24 25  26
                         a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p,  q, r, s, t, u, v, w, x, y,  z
const int legend[26] = { 1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10};
*/