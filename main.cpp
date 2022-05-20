#pragma GCC optimize("Ofast")
#define CATCH_CONFIG_RUNNER
#include "scl.h"
#include "Test_Folder/catch.hpp"
#include <chrono>

int main(int argc, char* argv[]){
    if(argc == 1) {
        Catch::Session().run();
        return 0;
    }

    std::cout.precision(5);

    std::string rack = "THASERL";    //blank tiles should be marked as a '?', and the letters within the rack need to be upper-case

    //// Building the CADS, setting the RMAC build path, and building the dictionary for each vectorizer **ONLY NEEDS TO BE DONE ONCE PER VECTORIZER**
    scl::HorizontalScrabbleVectorizer hReader;
    hReader.build_CADS_from(argv[2]);
    hReader.set_RMAC_build_path_dictionary();
    hReader.build_dictionaries_from(argv[2]);

    scl::VerticalScrabbleVectorizer vReader;
    vReader.build_dictionaries_from(argv[2]);
    vReader.set_RMAC_build_path_dictionary();
    vReader.build_CADS_from(argv[2]);

    //// Setting and building the word boards and perk-boards for each vectorizer
    hReader.build_board_from(argv[1]);
    hReader.validate_board();
    hReader.update_perkBoard();

    vReader.build_board_from(argv[1]);
    vReader.validate_board();
    vReader.update_perkBoard();

    // Printing current boards to console
    std::cout << std::endl << "Horizontal Vectorizer Board:" << std::endl;
    hReader.console_print_formatted_board();
    std::cout << std::endl << "Vertical Vectorizer Board:" << std::endl;
    vReader.console_print_formatted_board();
    std::cout << std::endl;

    for (int i = 0; i < 3; ++i) {
        //// Calculating timing metrics for word generation
        auto start = std::chrono::high_resolution_clock::now();
        hReader.set_rack(rack);
        vReader.set_rack(rack);

        vReader.search_for_intersecting_moves();    //<- searching for moves that intersect with another word on the current board
        vReader.search_for_tangential_moves();      //<- searching for moves that are only touching another word and never intersecting with another word

        hReader.search_for_intersecting_moves();
        hReader.search_for_tangential_moves();

        vReader.validate_generated_moves();         //<- the generated moves need to be filtered and cleaned so that no invalid moves remain
        hReader.validate_generated_moves();
        auto end = std::chrono::high_resolution_clock::now();
        auto time_in_seconds = std::chrono::duration<double>(end - start);

        std::cout << "====================================================" << std::endl << std::endl;
        std::cout << "Time taken by move generation: " << time_in_seconds.count() << " seconds" << std::endl << std::endl;


        //// Calculating and printing the information of the best word for each vectorizer to the console
        vReader.update_best_move();     //<- this updates the best move for each vectorizer
        hReader.update_best_move();

        int vPoints = vReader.points_of_best_boarded_move();
        scl::Tstring bestVWord = vReader.get_best_raw_boarded_move();
        int hPoints = hReader.points_of_best_boarded_move();
        scl::Tstring bestHWord = hReader.get_best_raw_boarded_move();

        std::cout << "Best Horizontal Word: " << bestHWord.to_string() << std::endl;
        std::cout << "\tPoints: " << hPoints << std::endl;
        std::cout << "\t(" << hReader.get_best_x() << ", " << hReader.get_best_y() << ')' << std::endl;

        std::cout << "Best Vertical Word: " << bestVWord.to_string() << std::endl;
        std::cout << "\tPoints: " << vPoints << std::endl;
        std::cout << "\t(" << vReader.get_best_x() << ", " << vReader.get_best_y() << ')' << std::endl;


        //// The following if-statement places the best word overall into both boards, making both boards have the same content
        if(hPoints > vPoints){
            std::cout << std::endl << "Horizontal word \"" << bestHWord.to_string() << "\" is the best word overall" << std::endl;

            // the "raw_place_boarded_word(scl::Tstring)" method places the passed scl::Tstring into the current board strictly based off of the coordinates of each scl::Tile in the scl::Tstring
            hReader.raw_place_boarded_word(bestHWord);

            // the "return_vertical_format()" and "return_horizontal_format()" methods return that a version of the invoking scl::Tstring that has been changed from its original format into the other format
            vReader.raw_place_boarded_word(bestHWord.return_vertical_format());
        }
        else{
            std::cout << std::endl << "Vertical word \"" << bestVWord.to_string() << "\" is the best word overall" << std::endl;

            vReader.raw_place_boarded_word(bestVWord);

            hReader.raw_place_boarded_word(bestVWord.return_horizontal_format());
        }
        vReader.update_perkBoard();   //<- both boards have changed, so their perk board must be updated
        hReader.update_perkBoard();


        //// Printing current boards to console
        std::cout << std::endl << "Horizontal Vectorizer Board:" << std::endl;
        hReader.console_print_formatted_board();
        std::cout << std::endl << "Vertical Vectorizer Board:" << std::endl;
        vReader.console_print_formatted_board();
        std::cout << std::endl;

        hReader.clear_all_moves();
        vReader.clear_all_moves();
    }

    return 0;
}