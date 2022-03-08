//
// Created by misc1 on 3/7/2022.
//

#include "catch.hpp"
#include "../HorizontalBoardReader/HorizontalBoardReader.h"
#include "../VerticalBoardReader/VerticalBoardReader.h"
#include <sstream>

TEST_CASE("Testing user-oriented methods", "[ScrabbleReader]"){
    string hand = "GRO?LEE";
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
        while (getline(strStr, cell, ',')){
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


    SECTION("ScrabbleReader::return_formatted_board() and ScrabbleReader::get_raw_board()" ) {
        vector<LString> testBoard = hReader.return_formatted_board();
        REQUIRE(hReader.return_formatted_board() == originalBoard);
        REQUIRE(hReader.get_raw_board() == originalBoard);

        vector<LString> testDummyBoard = vReader.return_formatted_board();

        for (int i = 0; i < 15; ++i) {
            cout << testDummyBoard[i].to_string() << endl;
        }
        for (int i = 0; i < 15; ++i) {
            cout << originalBoard[i].to_string() << endl;
        }

        REQUIRE(testDummyBoard == originalBoard);
    }
}
