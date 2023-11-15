#include <fstream>
#include <sstream>

#include "../Utils/HorizontalScrabbleVectorizer/HorizontalScrabbleVectorizer.h"
#include "../Utils/VerticalScrabbleVectorizer/VerticalScrabbleVectorizer.h"
#include "catch.hpp"

TEST_CASE("Testing std::vector<scl::ScrabbleWord>& get_all_moves_at(int x_, int y)", "ScrabbleVectorizer") {
    scl::HorizontalScrabbleVectorizer hReader(std::string("ASDFGHJ"));
    hReader.build_board_from("../Test_Folder/Board.csv");
    hReader.update_perkBoard();
    hReader.build_dictionaries_from("../Test_Folder/scrabble_word_list.txt");
    hReader.validate_board();
    hReader.build_CADS_from("../Test_Folder/scrabble_word_list.txt");
    hReader.set_RMAC_build_path_dictionary();
    hReader.clear_all_moves();

    scl::VerticalScrabbleVectorizer vReader(std::string("ASDFGHJ"));
    vReader.build_board_from("../Test_Folder/Board.csv");
    vReader.update_perkBoard();
    vReader.build_dictionaries_from("../Test_Folder/scrabble_word_list.txt");
    vReader.validate_board();
    vReader.build_CADS_from("../Test_Folder/scrabble_word_list.txt");
    vReader.set_RMAC_build_path_dictionary();
    vReader.clear_all_moves();

    vReader.search_for_intersecting_moves();
    vReader.search_for_tangential_moves();

    hReader.search_for_intersecting_moves();
    hReader.search_for_tangential_moves();

    vReader.validate_generated_moves();
    hReader.validate_generated_moves();

    for (int i = 0; i < 15; ++i) {
        for (int j = 0; j < 15; ++j) {
            for (auto& hword : hReader.get_all_moves_at(i, j)) {
                if (hword.IsEmpty()) {
                    continue;
                }
                REQUIRE(hword[0].x == i);
                REQUIRE(hword[0].y == j);
            }
            for (auto& vword : vReader.get_all_moves_at(i, j)) {
                if (vword.IsEmpty()) {
                    continue;
                }
                REQUIRE(vword[0].y == 14 - i);
                REQUIRE(vword[0].x == j);
            }
        }
    }
}

TEST_CASE("Testing CADS Usage and Speed", "[CADS]") {
    scl::CADS wordData("../Data/scrabble_word_list.txt");

    SECTION("Testing words with \'A\' as the first letter") {
        std::vector<scl::AnchoredString> data = wordData.at_with(2, 'A');
        for (auto& it : data) {
            REQUIRE(it.first[it.second] == 'A');
        }
    }

    SECTION("Testing words with \'S\' as the 5th tile of the 3rd row") {
        std::vector<scl::AnchoredString> data = wordData.at_with(4, 'S');
        for (auto& it : data) {
            REQUIRE(it.first[it.second] == 'S');
        }
    }

    SECTION("Testing all words formed within CADS") {
        for (int i = 0; i < 15; ++i) {
            for (int j = 0; j < 15; ++j) {
                for (int k = 0; k < 26; ++k) {
                    char curChar = 'A' + k;
                    std::vector<scl::AnchoredString> data = wordData.at_with(j, curChar);
                    for (auto& it : data) {
                        REQUIRE(it.first[it.second] == curChar);
                    }
                }
            }
        }
    }
}

TEST_CASE("Testing user-oriented methods", "[ScrabbleVectorizer]") {
    std::string rack = "GRO?LEE";
    scl::ScrabbleString word("ADAM");
    word[0].x = 10;
    word[1].x = 11;
    word[2].x = 12;
    word[3].x = 13;
    word[0].y = 3;
    word[1].y = 3;
    word[2].y = 3;
    word[3].y = 3;

    scl::HorizontalScrabbleVectorizer defaultReader(rack);
    defaultReader.build_board_from("../Test_Folder/Board.csv");

    scl::HorizontalScrabbleVectorizer hReader(rack);
    hReader.build_board_from("../Test_Folder/Board.csv");
    hReader.update_perkBoard();

    scl::VerticalScrabbleVectorizer vReader(rack);
    vReader.build_board_from("../Test_Folder/Board.csv");
    vReader.update_perkBoard();

    std::ifstream boardFile;
    boardFile.open("../Test_Folder/Board.csv");
    if (!boardFile.is_open())
        throw std::invalid_argument("could not open ../Test_Folder/Board.csv");

    std::vector<scl::ScrabbleString> originalBoard;
    std::string row;
    int rowCount = 0;
    while (boardFile.good()) {
        getline(boardFile, row);
        std::string cell;
        std::stringstream strStr(row);
        int cellCount = 0;
        scl::ScrabbleString rowVect;
        while (getline(strStr, cell, ',') && cellCount < 15) {
            if (!cell.empty() && isalpha(cell[0])) {
                rowVect += scl::Tile(cell[0], cellCount, rowCount, 1);
            } else {
                rowVect += scl::Tile(' ', cellCount, rowCount, 1);
            }
            cellCount++;
        }
        originalBoard.push_back(rowVect);
        rowCount++;
    }
    boardFile.close();

    char originalPerkBoard[15][15] = {{'3', ' ', ' ', 'B', ' ', ' ', ' ', '3', ' ', ' ', ' ', 'B', ' ', ' ', '3'},
                                      {' ', '2', ' ', ' ', ' ', 'C', ' ', ' ', ' ', 'C', ' ', ' ', ' ', '2', ' '},
                                      {' ', ' ', '2', ' ', ' ', ' ', 'B', ' ', 'B', ' ', ' ', ' ', '2', ' ', ' '},
                                      {'B', ' ', ' ', '2', ' ', ' ', ' ', 'B', ' ', ' ', ' ', '2', ' ', ' ', 'B'},
                                      {' ', ' ', ' ', ' ', '2', ' ', ' ', ' ', ' ', ' ', '2', ' ', ' ', ' ', ' '},
                                      {' ', 'C', ' ', ' ', ' ', 'C', ' ', ' ', ' ', 'C', ' ', ' ', ' ', 'C', ' '},
                                      {' ', ' ', 'B', ' ', ' ', ' ', 'B', ' ', 'B', ' ', ' ', ' ', 'B', ' ', ' '},
                                      {'3', ' ', ' ', 'B', ' ', ' ', ' ', '2', ' ', ' ', ' ', 'B', ' ', ' ', '3'},
                                      {' ', ' ', 'B', ' ', ' ', ' ', 'B', ' ', 'B', ' ', ' ', ' ', 'B', ' ', ' '},
                                      {' ', 'C', ' ', ' ', ' ', 'C', ' ', ' ', ' ', 'C', ' ', ' ', ' ', 'C', ' '},
                                      {' ', ' ', ' ', ' ', '2', ' ', ' ', ' ', ' ', ' ', '2', ' ', ' ', ' ', ' '},
                                      {'B', ' ', ' ', '2', ' ', ' ', ' ', 'B', ' ', ' ', ' ', '2', ' ', ' ', 'B'},
                                      {' ', ' ', '2', ' ', ' ', ' ', 'B', ' ', 'B', ' ', ' ', ' ', '2', ' ', ' '},
                                      {' ', '2', ' ', ' ', ' ', 'C', ' ', ' ', ' ', 'C', ' ', ' ', ' ', '2', ' '},
                                      {'3', ' ', ' ', 'B', ' ', ' ', ' ', '3', ' ', ' ', ' ', 'B', ' ', ' ', '3'}};
    for (int i = 0; i < 15; ++i) {
        for (int j = 0; j < 15; ++j) {
            if (originalBoard[i][j].letter != ' ')
                originalPerkBoard[i][j] = ' ';
        }
    }

    SECTION("ScrabbleVectorizer::return_formatted_board_copy() and ScrabbleVectorizer::get_raw_board()") {
        REQUIRE(hReader.return_formatted_board_copy() == originalBoard);
        REQUIRE(hReader.get_raw_board() == originalBoard);
        REQUIRE(vReader.return_formatted_board_copy() == originalBoard);
    }

    SECTION("ScrabbleVectorizer::return_formatted_char_board_copy()") {
        std::vector<std::string> testVBoard = vReader.return_formatted_char_board_copy();
        std::vector<std::string> testHBoard = hReader.return_formatted_char_board_copy();

        for (int i = 0; i < 15; ++i) {
            REQUIRE(testHBoard[i] == originalBoard[i].ToString());
            REQUIRE(testHBoard[i] == originalBoard[i].ToString());
        }
    }

    SECTION("ScrabbleVectorizer::return_formatted_perkBoard_copy()") {
        std::vector<std::string> testVPerks = vReader.return_formatted_perkBoard_copy();
        std::vector<std::string> testHPerks = hReader.return_formatted_perkBoard_copy();

        for (int i = 0; i < 15; ++i) {
            for (int j = 0; j < 15; ++j) {
                REQUIRE(testVPerks[i][j] == originalPerkBoard[i][j]);
                REQUIRE(testHPerks[i][j] == originalPerkBoard[i][j]);
            }
        }
    }

    SECTION("ScrabbleVectorizer::return_raw_char_board_copy()") {
        std::vector<std::string> testVBoard = vReader.return_raw_char_board_copy();
        std::vector<std::string> testHBoard = hReader.return_raw_char_board_copy();

        for (int i = 0; i < 15; ++i) {
            for (int j = 0; j < 15; ++j) {
                REQUIRE(testVBoard[i][j] == originalBoard[j][14 - i].letter);
                REQUIRE(testHBoard[i][j] == originalBoard[i][j].letter);
            }
        }
    }

    SECTION("ScrabbleVectorizer::return_raw_char_board_copy()") {
        std::vector<std::string> testVPerks = vReader.return_raw_perkBoard_copy();
        std::vector<std::string> testHPerks = hReader.return_raw_perkBoard_copy();

        for (int i = 0; i < 15; ++i) {
            for (int j = 0; j < 15; ++j) {
                REQUIRE(testVPerks[i][j] == originalPerkBoard[j][14 - i]);
                REQUIRE(testHPerks[i][j] == originalPerkBoard[i][j]);
            }
        }
    }
}

TEST_CASE("Testing manual board and perk-board setting", "[ScrabbleVectorizer]") {
    std::string rack = "POIAUD?";

    scl::HorizontalScrabbleVectorizer hReader(rack);
    hReader.build_board_from("../Test_Folder/Board.csv");
    scl::VerticalScrabbleVectorizer vReader(rack);
    vReader.build_board_from("../Test_Folder/Board.csv");
    std::vector<std::string> customPerkBoard = {{'3', ' ', ' ', 'B', ' ', ' ', ' ', '3', ' ', ' ', ' ', '|', ' ', ' ', '3'},
                                                {' ', '2', ' ', ' ', ' ', 'C', ' ', ' ', ' ', 'C', ' ', '|', ' ', '2', ' '},
                                                {' ', ' ', '2', ' ', ' ', ' ', 'B', ' ', 'B', ' ', ' ', '|', '2', ' ', ' '},
                                                {'B', ' ', ' ', '2', ' ', ' ', ' ', 'B', ' ', ' ', ' ', '|', ' ', ' ', 'B'},
                                                {'=', '=', '=', '=', '=', '=', '=', '=', '=', '=', '=', '|', '=', '=', '='},
                                                {' ', 'C', ' ', ' ', ' ', 'C', ' ', ' ', ' ', 'C', ' ', '|', ' ', 'C', ' '},
                                                {' ', ' ', 'B', ' ', ' ', ' ', 'B', ' ', 'B', ' ', ' ', '|', 'B', ' ', ' '},
                                                {'3', ' ', ' ', 'B', ' ', ' ', ' ', '2', ' ', ' ', ' ', '|', ' ', ' ', '3'},
                                                {' ', ' ', 'B', ' ', ' ', ' ', 'B', ' ', 'B', ' ', ' ', '|', 'B', ' ', ' '},
                                                {' ', 'C', ' ', ' ', ' ', 'C', ' ', ' ', ' ', 'C', ' ', '|', ' ', 'C', ' '},
                                                {' ', ' ', ' ', ' ', '2', ' ', ' ', ' ', ' ', ' ', '2', '|', ' ', ' ', ' '},
                                                {'B', ' ', ' ', '2', ' ', ' ', ' ', 'B', ' ', ' ', ' ', '|', ' ', ' ', 'B'},
                                                {' ', ' ', '2', ' ', ' ', ' ', 'B', ' ', 'B', ' ', ' ', '|', '2', ' ', ' '},
                                                {' ', '2', ' ', ' ', ' ', 'C', ' ', ' ', ' ', 'C', ' ', '|', ' ', '2', ' '},
                                                {'3', ' ', ' ', 'B', ' ', ' ', ' ', '3', ' ', ' ', ' ', '|', ' ', ' ', '3'}};

    std::vector<std::string> customBoard = {{' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
                                            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
                                            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'B', 'B', 'B', 'B', 'B', ' ', ' '},
                                            {' ', ' ', 'S', ' ', 'C', ' ', ' ', ' ', 'B', 'B', 'B', 'B', 'B', ' ', ' '},
                                            {' ', ' ', 'S', ' ', 'C', ' ', ' ', ' ', 'B', 'B', 'B', 'B', 'B', 'L', ' '},
                                            {' ', ' ', 'S', ' ', 'C', ' ', ' ', ' ', 'B', 'B', 'B', 'B', 'B', 'L', ' '},
                                            {' ', ' ', 'S', ' ', 'C', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 'L', ' '},
                                            {' ', ' ', 'S', ' ', 'C', ' ', 'R', 'A', ' ', ' ', ' ', ' ', ' ', 'L', ' '},
                                            {' ', ' ', 'S', ' ', 'C', ' ', 'R', 'A', ' ', ' ', ' ', ' ', ' ', 'L', ' '},
                                            {' ', ' ', 'S', ' ', 'C', ' ', 'R', 'A', ' ', ' ', ' ', ' ', ' ', 'L', ' '},
                                            {' ', ' ', 'S', ' ', 'C', ' ', 'R', 'A', ' ', ' ', ' ', ' ', ' ', 'L', ' '},
                                            {' ', ' ', ' ', ' ', ' ', ' ', ' ', 'A', ' ', ' ', ' ', ' ', ' ', 'L', ' '},
                                            {' ', ' ', ' ', ' ', ' ', ' ', ' ', 'A', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
                                            {' ', ' ', ' ', ' ', ' ', ' ', ' ', 'A', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
                                            {' ', ' ', ' ', ' ', ' ', ' ', ' ', 'A', ' ', ' ', ' ', ' ', ' ', ' ', ' '}};

    SECTION("ScrabbleVectorizer::build_board_from(const std::vector<std::string>&)") {
        vReader.build_board_from(customBoard);
        hReader.build_board_from(customBoard);

        REQUIRE(vReader.return_formatted_board_copy() == hReader.return_formatted_board_copy());
    }
}