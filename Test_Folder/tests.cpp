//
// Created by misc1 on 3/7/2022.
//

#include "catch.hpp"
#include "../HorizontalBoardReader/HorizontalBoardReader.h"
#include "../VerticalBoardReader/VerticalBoardReader.h"
#include <sstream>

TEST_CASE("Testing user-oriented methods", "[ScrabbleReader]"){
    string hand = "GRO?LEE";
    LString word = "ADAM";
    word[0].x = 10;
    word[1].x = 11;
    word[2].x = 12;
    word[3].x = 13;
    word[0].y = 3;
    word[1].y = 3;
    word[2].y = 3;
    word[3].y = 3;

    HorizontalBoardReader defaultReader(hand);
    defaultReader.build_board("../Test_Folder/Board.csv");

    HorizontalBoardReader hReader(hand);
    hReader.build_board("../Test_Folder/Board.csv");

    VerticalBoardReader vReader(hand);
    vReader.build_board("../Test_Folder/Board.csv");

    ifstream boardFile;
    boardFile.open("../Test_Folder/Board.csv");
    if(!boardFile.is_open())
        throw invalid_argument("could not open ../Test_Folder/Board.csv");

    vector<LString> originalBoard;
    string row;
    int rowCount = 0;
    while (boardFile.good()){
        getline(boardFile, row);
        string cell;
        stringstream strStr(row);
        int cellCount = 0;
        LString rowVect;
        while (getline(strStr, cell, ',') && cellCount < 15){
            if(!cell.empty() && isalpha(cell[0])) {
                rowVect.push_back(Letter(cell[0], cellCount, rowCount, 1));
            }
            else {
                rowVect.push_back(Letter(' ', cellCount, rowCount, 1));
            }
            cellCount++;
        }
        originalBoard.push_back(rowVect);
        rowCount++;
    }
    boardFile.close();

    char originalPerkBoard[15][15] =   {{'3', ' ', ' ', 'B', ' ', ' ', ' ', '3', ' ', ' ', ' ', 'B', ' ', ' ', '3'},
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
            if(originalBoard[i][j].LData != ' ')
                originalPerkBoard[i][j] = ' ';
        }
    }


    SECTION("ScrabbleReader::return_formatted_board() and ScrabbleReader::get_raw_board()") {
        REQUIRE(hReader.return_formatted_board() == originalBoard);
        REQUIRE(hReader.get_raw_board() == originalBoard);
        REQUIRE(vReader.return_formatted_board() == originalBoard);
    }

    SECTION("ScrabbleReader::set_board()") {
        hReader.set_board(originalBoard);
        vReader.set_board(originalBoard);

        REQUIRE(hReader.return_formatted_board() == originalBoard);
        REQUIRE(hReader.get_raw_board() == originalBoard);
        REQUIRE(vReader.return_formatted_board() == originalBoard);
    }

    SECTION("ScrabbleReader::return_formatted_board_with()") {
        defaultReader.place_into_board(word);
        vector<LString> testBoard = defaultReader.return_formatted_board();

        REQUIRE(hReader.return_formatted_board_with(word) == testBoard);
        REQUIRE(vReader.return_formatted_board_with(word) == testBoard);
    }

    SECTION("ScrabbleReader::return_formatted_char_board(char**)") {
        vector<string> testVBoard = vReader.return_formatted_char_board();
        vector<string> testHBoard = hReader.return_formatted_char_board();

        for (int i = 0; i < 15; ++i) {
            REQUIRE(testHBoard[i] == originalBoard[i].to_string());
            REQUIRE(testHBoard[i] == originalBoard[i].to_string());
        }
    }

    SECTION("ScrabbleReader::return_formatted_perkBoard()") {
        vector<string> testVPerks = vReader.return_formatted_perkBoard();
        vector<string> testHPerks = hReader.return_formatted_perkBoard();

        for (int i = 0; i < 15; ++i) {
            for (int j = 0; j < 15; ++j) {
                REQUIRE(testVPerks[i][j] == originalPerkBoard[i][j]);
                REQUIRE(testHPerks[i][j] == originalPerkBoard[i][j]);
            }
        }
    }

    SECTION("ScrabbleReader::return_raw_char_board()") {
        vector<string> testVBoard = vReader.return_raw_char_board();
        vector<string> testHBoard = hReader.return_raw_char_board();

        for (int i = 0; i < 15; ++i) {
            for (int j = 0; j < 15; ++j) {
                REQUIRE(testVBoard[i][j] == originalBoard[j][14 - i].LData);
                REQUIRE(testHBoard[i][j] == originalBoard[i][j].LData);
            }
        }
    }

    SECTION("ScrabbleReader::return_raw_char_board()") {
        vector<string> testVPerks = vReader.return_raw_perkBoard();
        vector<string> testHPerks = hReader.return_raw_perkBoard();

        for (int i = 0; i < 15; ++i) {
            for (int j = 0; j < 15; ++j) {
                REQUIRE(testVPerks[i][j] == originalPerkBoard[j][14 - i]);
                REQUIRE(testHPerks[i][j] == originalPerkBoard[i][j]);
            }
        }
    }
}
