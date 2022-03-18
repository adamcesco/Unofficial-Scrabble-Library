#include "catch.hpp"
#include "../HorizontalScrabbleVectorizer/HorizontalScrabbleVectorizer.h"
#include "../VerticalScrabbleVectorizer/VerticalScrabbleVectorizer.h"
#include <sstream>

TEST_CASE("Testing TString::erase_at(int)", "[TString]"){
    SECTION("Testing a populated TString"){
        TString testDummy = "Adam";
        testDummy.erase_at(2);
        REQUIRE(testDummy == "Adm");

        bool error_thrown = false;
        try{
            TString testDummy = "Adam";
            testDummy.erase_at(-1);
        } catch(const invalid_argument& e){
            error_thrown = true;
        }
        REQUIRE(error_thrown);

        error_thrown = false;
        try{
            TString testDummy = "Adam";
            testDummy.erase_at(4);
        } catch(const invalid_argument& e){
            error_thrown = true;
        }
        REQUIRE(error_thrown);
    }

    SECTION("Testing an empty TString"){
        bool error_thrown = false;
        try{
            TString testDummy;
            testDummy.erase_at(0);
        } catch(const invalid_argument& e){
            error_thrown = true;
        }
        REQUIRE(error_thrown);
    }
}

TEST_CASE("Testing ScrabbleDataset Usage and Speed", "[ScrabbleDataset]"){
    ScrabbleDataset wordData("../Data/scrabble_word_list.txt");

    SECTION("Testing words with \'A\' as the first letter"){
        vector<AnchoredString> data = wordData.return_this_at(2, 2, 'A');
        for (auto& it : data) {
            REQUIRE(it.first[it.second] == 'A');
        }
    }

    SECTION("Testing words with \'S\' as the 5th tile of the 3rd row"){
        vector<AnchoredString> data = wordData.return_this_at(2, 4, 'S');
        for (auto& it : data) {
            REQUIRE(it.first[it.second] == 'S');
        }
    }

    SECTION("Testing all words formed within ScrabbleDataset"){
        for (int i = 0; i < 15; ++i) {
            for (int j = 0; j < 15; ++j) {
                for (int k = 0; k < 26; ++k) {
                    char curChar = 'A' + k;
                    vector<AnchoredString> data = wordData.return_this_at(i, j, curChar);
                    for (auto& it : data) {
                        REQUIRE(it.first[it.second] == curChar);
                    }
                }
            }
        }
    }
}

TEST_CASE("Testing user-oriented methods", "[ScrabbleVectorizer]"){
    string hand = "GRO?LEE";
    TString word = "ADAM";
    word[0].x = 10;
    word[1].x = 11;
    word[2].x = 12;
    word[3].x = 13;
    word[0].y = 3;
    word[1].y = 3;
    word[2].y = 3;
    word[3].y = 3;

    HorizontalScrabbleVectorizer defaultReader(hand);
    defaultReader.build_board("../Test_Folder/Board.csv");

    HorizontalScrabbleVectorizer hReader(hand);
    hReader.build_board("../Test_Folder/Board.csv");

    VerticalScrabbleVectorizer vReader(hand);
    vReader.build_board("../Test_Folder/Board.csv");

    ifstream boardFile;
    boardFile.open("../Test_Folder/Board.csv");
    if(!boardFile.is_open())
        throw invalid_argument("could not open ../Test_Folder/Board.csv");

    vector<TString> originalBoard;
    string row;
    int rowCount = 0;
    while (boardFile.good()){
        getline(boardFile, row);
        string cell;
        stringstream strStr(row);
        int cellCount = 0;
        TString rowVect;
        while (getline(strStr, cell, ',') && cellCount < 15){
            if(!cell.empty() && isalpha(cell[0])) {
                rowVect += Tile(cell[0], cellCount, rowCount, 1);
            }
            else {
                rowVect += Tile(' ', cellCount, rowCount, 1);
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
            if(originalBoard[i][j].letter != ' ')
                originalPerkBoard[i][j] = ' ';
        }
    }


    SECTION("ScrabbleVectorizer::return_formatted_board_copy() and ScrabbleVectorizer::get_raw_board()") {
        REQUIRE(hReader.return_formatted_board_copy() == originalBoard);
        REQUIRE(hReader.get_raw_board() == originalBoard);
        REQUIRE(vReader.return_formatted_board_copy() == originalBoard);
    }

    SECTION("ScrabbleVectorizer::set_board(const vector<TString> &passed)") {
        hReader.set_board(originalBoard);
        vReader.set_board(originalBoard);

        REQUIRE(hReader.return_formatted_board_copy() == originalBoard);
        REQUIRE(hReader.get_raw_board() == originalBoard);
        REQUIRE(vReader.return_formatted_board_copy() == originalBoard);

        vector<TString> vboardFormattedCopy = vReader.return_formatted_board_copy();
        vector<TString> hboardFormattedCopy = hReader.return_formatted_board_copy();
        for (int i = 0; i < 15; ++i) {
            for (int j = 0; j < 15; ++j) {
                REQUIRE(vboardFormattedCopy[i][j].x == i);
                REQUIRE(vboardFormattedCopy[i][j].y == 14 - j);
                REQUIRE(hboardFormattedCopy[i][j].x == j);
                REQUIRE(hboardFormattedCopy[i][j].y == i);
            }
        }
    }

    SECTION("ScrabbleVectorizer::return_formatted_char_board_copy()") {
        vector<string> testVBoard = vReader.return_formatted_char_board_copy();
        vector<string> testHBoard = hReader.return_formatted_char_board_copy();

        for (int i = 0; i < 15; ++i) {
            REQUIRE(testHBoard[i] == originalBoard[i].to_string());
            REQUIRE(testHBoard[i] == originalBoard[i].to_string());
        }
    }

    SECTION("ScrabbleVectorizer::return_formatted_perkBoard_copy()") {
        vector<string> testVPerks = vReader.return_formatted_perkBoard_copy();
        vector<string> testHPerks = hReader.return_formatted_perkBoard_copy();

        for (int i = 0; i < 15; ++i) {
            for (int j = 0; j < 15; ++j) {
                REQUIRE(testVPerks[i][j] == originalPerkBoard[i][j]);
                REQUIRE(testHPerks[i][j] == originalPerkBoard[i][j]);
            }
        }
    }

    SECTION("ScrabbleVectorizer::return_raw_char_board_copy()") {
        vector<string> testVBoard = vReader.return_raw_char_board_copy();
        vector<string> testHBoard = hReader.return_raw_char_board_copy();

        for (int i = 0; i < 15; ++i) {
            for (int j = 0; j < 15; ++j) {
                REQUIRE(testVBoard[i][j] == originalBoard[j][14 - i].letter);
                REQUIRE(testHBoard[i][j] == originalBoard[i][j].letter);
            }
        }
    }

    SECTION("ScrabbleVectorizer::return_raw_char_board_copy()") {
        vector<string> testVPerks = vReader.return_raw_perkBoard_copy();
        vector<string> testHPerks = hReader.return_raw_perkBoard_copy();

        for (int i = 0; i < 15; ++i) {
            for (int j = 0; j < 15; ++j) {
                REQUIRE(testVPerks[i][j] == originalPerkBoard[j][14 - i]);
                REQUIRE(testHPerks[i][j] == originalPerkBoard[i][j]);
            }
        }
    }
}

TEST_CASE("Testing method TString::is_descendent_of", "[TString]"){
    string givenHand = "DOEWJ?K";

    SECTION("Testing different variations of the given word \"DOEWJ?K\" as input"){
        TString toValidate = "JOOKED";
        REQUIRE(toValidate.is_descendent_of(givenHand));

        toValidate = "JOUKED";
        REQUIRE(toValidate.is_descendent_of(givenHand));

        toValidate = "JOWLED";
        REQUIRE(toValidate.is_descendent_of(givenHand));

        toValidate = "WORKED";
        REQUIRE(toValidate.is_descendent_of(givenHand));

        toValidate = "WORK";
        REQUIRE(toValidate.is_descendent_of(givenHand));

        toValidate = "JOKEY";
        REQUIRE(toValidate.is_descendent_of(givenHand));

        toValidate = "JOKERS";
        REQUIRE(!toValidate.is_descendent_of(givenHand));

        toValidate = "JOKING";
        REQUIRE(!toValidate.is_descendent_of(givenHand));

        toValidate = "";
        REQUIRE(!toValidate.is_descendent_of(givenHand));
    }
}

TEST_CASE("Testing method TString::row_is_descendent_of", "[TString]"){
    string givenHand = "LEO?UDQ";
    TString givenRow = "     R B  D    ";
    givenRow.set_x_vals_to_subscripts();

    SECTION("Testing different variations of the given word \"LEO?UDQ\" with row \"     R B  D    \" as input"){
        TString toValidate = "     R BUDDY   ";
        TString givenWord = "BUDDY";
        givenWord.set_x_vals_to_subscripts();
        givenWord.add_to_x_vals(7);
        REQUIRE(toValidate.row_is_descendent_of(givenHand, givenRow, givenWord));

        toValidate = "     ROB  D    ";
        givenWord = "ROB";
        givenWord.set_x_vals_to_subscripts();
        givenWord.add_to_x_vals(5);
        REQUIRE(toValidate.row_is_descendent_of(givenHand, givenRow, givenWord));

        toValidate = "     R BLED    ";
        givenWord = "BLED";
        givenWord.set_x_vals_to_subscripts();
        givenWord.add_to_x_vals(7);
        REQUIRE(toValidate.row_is_descendent_of(givenHand, givenRow, givenWord));

        toValidate = "     R B  DAD  ";
        givenWord = "DAD";
        givenWord.set_x_vals_to_subscripts();
        givenWord.add_to_x_vals(10);
        REQUIRE(toValidate.row_is_descendent_of(givenHand, givenRow, givenWord));

        toValidate = "     RUB  D    ";
        givenWord = "RUB";
        givenWord.set_x_vals_to_subscripts();
        givenWord.add_to_x_vals(5);
        REQUIRE(toValidate.row_is_descendent_of(givenHand, givenRow, givenWord));

        toValidate = "     R BADDY   ";
        givenWord = "BADDY";
        givenWord.set_x_vals_to_subscripts();
        givenWord.add_to_x_vals(7);
        REQUIRE(!toValidate.row_is_descendent_of(givenHand, givenRow, givenWord));

        toValidate = "     R B  DUCK ";
        givenWord = "DUCK";
        givenWord.set_x_vals_to_subscripts();
        givenWord.add_to_x_vals(10);
        REQUIRE(!toValidate.row_is_descendent_of(givenHand, givenRow, givenWord));

        toValidate = "";
        givenWord = "ROB";
        givenWord.set_x_vals_to_subscripts();
        givenWord.add_to_x_vals(5);
        REQUIRE(!toValidate.row_is_descendent_of(givenHand, givenRow, givenWord));
    }
}

TEST_CASE("Testing manual board and perk-board setting", "[ScrabbleVectorizer]"){
    string hand = "POIAUD?";

    HorizontalScrabbleVectorizer hReader(hand);
    hReader.build_board("../Test_Folder/Board.csv");
    hReader.validate_board();
    VerticalScrabbleVectorizer vReader(hand);
    vReader.build_board("../Test_Folder/Board.csv");
    vReader.validate_board();
    vector<string> customPerkBoard =   {{'3', ' ', ' ', 'B', ' ', ' ', ' ', '3', ' ', ' ', ' ', '|', ' ', ' ', '3'},
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

    vector<string> customBoard =       {{' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
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

    SECTION("ScrabbleVectorizer::set_perkBoard(const vector<string>&)"){
        vReader.set_perkBoard(customPerkBoard);
        hReader.set_perkBoard(customPerkBoard);

        REQUIRE(vReader.return_formatted_perkBoard_copy() == hReader.return_formatted_perkBoard_copy());
    }

    SECTION("ScrabbleVectorizer::set_board(const vector<string>&)"){
        vReader.set_board(customBoard);
        hReader.set_board(customBoard);

        REQUIRE(vReader.return_formatted_board_copy() == hReader.return_formatted_board_copy());
    }
}