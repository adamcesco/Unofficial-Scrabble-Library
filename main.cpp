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

    std::cout.precision(9);

    std::string rack = "ADHSERK";    //blank tiles should be marked as a '?', and the letter within the rack need to be upper-case

    ////Building the CADS, setting the RMAC build path, and building the dictionary ONLY NEEDS TO BE DONE ONCE PER VECTORIZER
    scl::HorizontalScrabbleVectorizer hReader;  //now building HorizontalScrabbleVectorizer (only generates horizontal moves)
    hReader.build_CADS_from(argv[2]);           //<- This tells the vectorizer to build the CADS from the given file directory
    hReader.set_RMAC_build_path_dictionary();   //<- Meaning that everytime we build an RMAC, it is built from this vectorizer's dictionary
    hReader.build_dictionaries_from(argv[2]);   //<- This tells the vectorizer to build it's dictionary from the given file

    ////Building the CADS, setting the RMAC build path, and building the dictionary ONLY NEEDS TO BE DONE ONCE PER VECTORIZER
    scl::VerticalScrabbleVectorizer vReader;    //Now building VerticalScrabbleVectorizer (only generates vertical moves)
    vReader.build_dictionaries_from(argv[2]);   //every vectorizer needs to have a built dictionary, set their RMAC dictionary build path, and have a built CADS
    vReader.set_RMAC_build_path_dictionary();
    vReader.build_CADS_from(argv[2]);

    hReader.build_board_from(argv[1]);          //<- This builds the board from the given file
    hReader.validate_board();                   //<- This validates all vertical and horizontal words on the current board
    hReader.prep_perkBoard();                   //<- This prepares the perk-board based off of the current board

    vReader.build_board_from(argv[1]);          //every vectorizer needs a built board, a validated board, and a prepared perk-board
    vReader.validate_board();
    vReader.prep_perkBoard();

    std::cout << std::endl;
    hReader.console_print_formatted_board();    //Printing current boards to console
    std::cout << std::endl << "----------------------------------" << std::endl << std::endl;
    vReader.console_print_formatted_board();
    std::cout << std::endl;

    auto start = std::chrono::high_resolution_clock::now();  //Timing word generation
    hReader.set_rack(rack);
    vReader.set_rack(rack);

    vReader.search_for_intersecting_moves();    //<- Searching for moves that intersect with another word on the current board
    vReader.search_for_tangential_moves();      //<- Searching for moves that are only touching another word and never intersecting with another word

    hReader.search_for_intersecting_moves();
    hReader.search_for_tangential_moves();

    vReader.validate_generated_moves();         //<- The generated moves need to be filtered and cleaned so that no invalid moves are left
    hReader.validate_generated_moves();
    auto end = std::chrono::high_resolution_clock::now();   //Move generation is completed, ending timer here
    auto time_in_seconds = std::chrono::duration<double>(end - start);
    std::cout << "Time taken by move generation: " << time_in_seconds.count() << " seconds" << std::endl << std::endl;

    vReader.update_best_move();     //<- This updates the best move for each vectorizer to be the move that has the highest amount of points
    hReader.update_best_move();

    int vPoints = vReader.points_of_best_boarded_move();            //<- The "points_of_best_boarded_move()" method gets the points of the boarded version of the best move: counting adjacently formed words and board perks
    scl::Tstring bestVWord = vReader.get_best_raw_boarded_move();
    int hPoints = hReader.points_of_best_boarded_move();
    scl::Tstring bestHWord = hReader.get_best_raw_boarded_move();

    //printing the information of the best word for the board to the console
    std::cout << "Best Horizontal Word: " << bestHWord.to_string() << std::endl;
    std::cout << "\tPoints: " << hPoints << std::endl;
    std::cout << "\t(" << hReader.get_best_x() << ", " << hReader.get_best_y() << ')' << std::endl;
    std::cout << "Best Vertical Word: " << bestVWord.to_string() << std::endl;
    std::cout << "\tPoints: " << vPoints << std::endl;
    std::cout << "\t(" << vReader.get_best_x() << ", " << vReader.get_best_y() << ')' << std::endl;

    //placing each move into the board
    vReader.raw_place_boarded_word(bestVWord);      //<- the "raw_place_boarded_word(scl::Tstring)" method places the passed scl::Tstring into the current board strictly based off of the coordinates of each scl::Tile in the scl::Tstring
    vReader.prep_perkBoard();                       //<- the board was changed, so the prep board must be updated
    hReader.raw_place_boarded_word(bestHWord);
    hReader.prep_perkBoard();

    std::cout << std::endl;
    hReader.console_print_formatted_board();
    std::cout << std::endl << "----------------------------------" << std::endl << std::endl;
    vReader.console_print_formatted_board();
    std::cout << std::endl;

    hReader.clear_all_moves();
    vReader.clear_all_moves();

    //TODO: add a joiner method that joins the two vectorizers to have the same board

    return 0;
}