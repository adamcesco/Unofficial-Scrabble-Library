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

    cout.precision(9);

    string rack = "ADEGOSS";    //blank tiles should be marked as a '?'

    HorizontalScrabbleVectorizer hReader(rack);
    hReader.build_board(argv[1]);
    hReader.validate_board();
    hReader.console_print_formatted_board();
    hReader.clear_wordSets();

//    VerticalScrabbleVectorizer vReader(rack);
//    vReader.build_board(argv[1]);
//    vReader.validate_board();
//    vReader.console_print_formatted_board();
//    vReader.clear_wordSets();

    auto start = chrono::high_resolution_clock::now();  //start timer here (timing word generation)
//    vReader.search_for_intersecting_words();
//    vReader.search_for_tangential_words();

//    hReader.search_for_intersecting_words();
    hReader.search_for_tangential_words();

//    vReader.validate_words();           //uses ~0.03 seconds for horizontal and vertical word validation
    hReader.validate_words();
    auto end = chrono::high_resolution_clock::now();   //end timer here
    auto time_in_seconds = chrono::duration<double>(end - start);
    cout << "Time taken by move generation: " << time_in_seconds.count() << " seconds" << endl;

//    vReader.update_best_word();
    hReader.update_best_word();

//    int vPoints = vReader.points_of_best_word();
//    TString bestVWord = vReader.get_best_word();
    int hPoints = hReader.points_of_best_word();
    TString bestHWord = hReader.get_best_word();

    //printing the information of the best word for the board to the console
    cout << "Best Horizontal Word: " << bestHWord.to_string() << endl;
    cout << "\tPoints: " << hPoints << endl;
//    cout << "Best Vertical Word: " << bestVWord.to_string() << endl;
//    cout << "\tPoints: " << vPoints << endl;

//    cout << "Best word for the board: ";
//    if (hPoints > vPoints) {
//        cout << bestHWord.to_string() << endl;
//        cout << "\tPoints: " << hPoints << endl;
//        cout << "\thorizontal" << endl;
//    } else {
//        cout << bestVWord.to_string() << endl;
//        cout << "\tPoints: " << vPoints << endl;
//        cout << "\tvertical" << endl;
//    }

    //printing to board
//    vReader.place_into_board(bestVWord);
    hReader.place_into_board(bestHWord);
    hReader.console_print_formatted_board();
//    vReader.console_print_formatted_board();

    return 0;
}