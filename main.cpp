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

    std::cout.precision(9);

    std::string rack = "AEORITE";    //blank tiles should be marked as a '?'

    scl::HorizontalScrabbleVectorizer hReader(rack);     //building HorizontalScrabbleVectorizer
    hReader.build_board_from(argv[1]);
    hReader.prep_perkBoard();
    hReader.build_dictionaries_from(argv[2]);
    hReader.validate_board();
    hReader.build_CADS_from(argv[2]);
    hReader.set_RMAC_build_path_dictionary();
    hReader.clear_all_moves();

    scl::VerticalScrabbleVectorizer vReader(rack);       //building VerticalScrabbleVectorizer
    vReader.build_board_from(argv[1]);
    vReader.prep_perkBoard();
    vReader.build_dictionaries_from(argv[2]);
    vReader.validate_board();
    vReader.build_CADS_from(argv[2]);
    vReader.set_RMAC_build_path_dictionary();
    vReader.clear_all_moves();

    hReader.console_print_formatted_board();        //printing boards
    vReader.console_print_formatted_board();

    auto start = std::chrono::high_resolution_clock::now();  //start timer here (timing word generation)
    vReader.search_for_intersecting_words();
    vReader.search_for_tangential_words();

    hReader.search_for_intersecting_words();
    hReader.search_for_tangential_words();

    vReader.validate_words();
    hReader.validate_words();
    auto end = std::chrono::high_resolution_clock::now();   //end timer here
    auto time_in_seconds = std::chrono::duration<double>(end - start);
    std::cout << "Time taken by move generation: " << time_in_seconds.count() << " seconds" << std::endl;

    vReader.update_best_word();
    hReader.update_best_word();

    int vPoints = vReader.points_of_best_word();
    scl::Tstring bestVWord = vReader.get_best_word();
    int hPoints = hReader.points_of_best_word();
    scl::Tstring bestHWord = hReader.get_best_word();

    //printing the information of the best word for the board to the console
    std::cout << "Best Horizontal Word: " << bestHWord.to_string() << std::endl;
    std::cout << "\tPoints: " << hPoints << std::endl;
    std::cout << '\t' << '(' << hReader.get_best_x() << ", " << hReader.get_best_y() << ')' << std::endl;
    std::cout << "Best Vertical Word: " << bestVWord.to_string() << std::endl;
    std::cout << "\tPoints: " << vPoints << std::endl;
    std::cout << '\t' << '(' << vReader.get_best_x() << ", " << vReader.get_best_y() << ')' << std::endl;
//
    std::cout << "Best word for the board: ";
    if (hPoints > vPoints) {
        std::cout << bestHWord.to_string() << std::endl;
        std::cout << "\tPoints: " << hPoints << std::endl;
        std::cout << "\thorizontal" << std::endl;
    } else {
        std::cout << bestVWord.to_string() << std::endl;
        std::cout << "\tPoints: " << vPoints << std::endl;
        std::cout << "\tvertical" << std::endl;
    }

    //printing to board
    vReader.place_into_board(bestVWord);
    hReader.place_into_board(bestHWord);
    hReader.console_print_formatted_board();
    std::cout << "----------------------------------" << std::endl;
    vReader.console_print_formatted_board();

    return 0;
}