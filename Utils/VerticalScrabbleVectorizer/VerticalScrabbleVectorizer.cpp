#include "VerticalScrabbleVectorizer.h"
#include <sstream>
#include <vector>
#include <iostream>
#include <fstream>

void scl::VerticalScrabbleVectorizer::build_board_from(const char* fileDir) {
    std::ifstream boardFile;
    boardFile.open(fileDir);
    if(!boardFile.is_open())
        throw std::invalid_argument("could not open file path passed to void scl::VerticalScrabbleVectorizer::build_board_from(const char* fileDir)");

    std::string row;
    board.clear();
    int rowCount = 0;
    while (boardFile.good()){
        getline(boardFile, row);
        std::string cell;
        std::stringstream strStr(row);
        int cellCount = 0;
        scl::Tstring LRow;
        while (getline(strStr, cell, ',') && cellCount < 15){
            if(!cell.empty() && isalpha(cell[0])) {
                LRow += scl::Tile(cell[0], cellCount, rowCount, 1);
            }
            else {
                LRow += scl::Tile(' ', cellCount, rowCount, 1);
            }
            cellCount++;
        }
        board.push_back(LRow);
        rowCount++;
    }
    boardFile.close();

    std::vector<scl::Tstring> boardCpy;
    for (int i = 14; i >= 0; i--) {  //i = x
        scl::Tstring column;
        for (int j = 0; j < 15; j++) {  //j = y
            column += scl::Tile(board[j][i].letter, j, 14 - i, 1);
        }
        boardCpy.push_back(column);
    }
    board = boardCpy;
}

void scl::VerticalScrabbleVectorizer::console_print_formatted_board() const{
    for (int i = 0; i < 15; ++i) {
        for (int j = 14; j >= 0; --j) {
            std::cout << board[j].read_at(i).letter;
        }
        std::cout << std::endl;
    }
}

scl::Tstring scl::VerticalScrabbleVectorizer::update_best_move(){
//    int rowSubscript = 0;
    bestWord.clear();
    for (int i = 0; i < 15; ++i) {
        for (int j = 0; j < 15; ++j) {
            for (const auto& word: moveSets[i][j]) {
                int wordPoints = points_of_raw_boarded_tstr(word) + points_of_raw_boarded_tstr(word);
                int bestWordPoints = points_of_raw_boarded_tstr(bestWord) + points_of_raw_boarded_tstr(bestWord);

                if (wordPoints > bestWordPoints) {
                    bestWord = word;
                    //x = y, y = 14 - x
                    bestX = (14 - i);
                    bestY = j;
                } else if (wordPoints != 0 && wordPoints == bestWordPoints) {
                    if (word.length() < bestWord.length() || bestWord.is_empty()) {
                        bestWord = word;
                        //x = y, y = 14 - x
                        bestX = (14 - i);
                        bestY = j;
                    }
                }
            }
        }
    }

    return bestWord;
}

void scl::VerticalScrabbleVectorizer::validate_generated_moves() {
    for (int i = 0; i < 15; ++i) {
        for (int j = 0; j < 15; ++j) {
            for (auto& word: moveSets[i][j]) {
                std::vector<scl::Tstring> boardCpy = return_raw_board_with(word);

                for (int k = 0; k < 15; k++) {
                    scl::Tstring row;
                    scl::Tstring column;
                    for (int l = 0; l < 15; ++l) {
                        row += boardCpy[k].read_at(l);
                        column += boardCpy[14 - l].read_at(k);
                    }

                    std::vector<std::string> colShards = column.string_fragments();

                    for (const auto& shard : colShards) {
                        if(shard.length() > 1 && dictionary.find(shard) == dictionary.end()) {
                            word.clear();
                        }
                    }

                    std::vector<std::string> rowShards = row.string_fragments();

                    for (const auto& shard : rowShards) {
                        if(shard.length() > 1 && dictionary.find(shard) == dictionary.end()) {
                            word.clear();
                        }
                    }
                }
            }
        }
    }
}

void scl::VerticalScrabbleVectorizer::validate_board() const{
    if(dictionary.empty())
        throw std::invalid_argument("Error in ScrabbleVectorizer::validate_board() | unordered_map<scl::Tstring> dictionary is empty.");

    for (int i = 0; i < 15; ++i) {
        scl::Tstring column;
        for (int j = 0; j < 15; ++j) {
            column += board[i].read_at(j);
        }

        scl::Tstring row;
        for (int j = 14; j >= 0; --j) {
            row += board[j].read_at(i);
        }

        std::vector<std::string> colShards = column.string_fragments();

        for (const auto& shard : colShards) {
            if(shard.length() > 1 && dictionary.find(shard) == dictionary.end())
                throw std::invalid_argument("Error in void scl::VerticalScrabbleVectorizer::validate_board() | Invalid vertical Word in Data/Board.csv |" + shard + '|');
        }

        std::vector<std::string> rowShards = row.string_fragments();

        for (const auto& shard : rowShards) {
            if(shard.length() > 1 && dictionary.find(shard) == dictionary.end())
                throw std::invalid_argument("Error in void scl::VerticalScrabbleVectorizer::validate_board() | Invalid horizontal Word in Data/Board.csv |" + shard + '|');
        }
    }
}

std::vector<std::string> scl::VerticalScrabbleVectorizer::return_formatted_perkBoard_copy() const {
    std::vector<std::string> toReturn;
    for (int i = 0; i < 15; ++i) {
        std::string column;
        for (int j = 14; j >= 0; --j) {
            column += perkBoard[j][i];
        }
        toReturn.push_back(column);
    }
    return toReturn;
}

std::vector<std::string> scl::VerticalScrabbleVectorizer::return_formatted_char_board_copy() const {
    std::vector<std::string> boardCpy;
    for (int i = 0; i < 15; ++i) {
        std::string column;
        for (int j = 14; j >= 0; --j) {
            column += board[j].read_at(i).letter;
        }
        boardCpy.push_back(column);
    }
    return boardCpy;
}

std::vector<scl::Tstring> scl::VerticalScrabbleVectorizer::return_formatted_board_copy() const {
    std::vector<scl::Tstring> boardCpy;
    for (int i = 0; i < 15; ++i) {
        scl::Tstring column;
        for (int j = 14; j >= 0; --j)
            column += board[j].read_at(i);
        boardCpy.push_back(column);
    }
    return boardCpy;
}

void scl::VerticalScrabbleVectorizer::build_board_from(const std::vector<std::string> &passed) {
    if(passed.size() != 15)
        throw std::invalid_argument("Error in scl::VerticalScrabbleVectorizer::build_board_from(std::vector<std::string> passed) | passed argument is not of a proper size.");

    std::vector<scl::Tstring> boardCpy;
    for (int i = 14; i >= 0; --i) {  //i = x
        if(passed[i].length() != 15)
            throw std::invalid_argument("Error in scl::VerticalScrabbleVectorizer::build_board_from(std::vector<std::string> passed) | passed argument has an element that is not of a proper size.");
        scl::Tstring column;
        for (int j = 0; j < 15; j++) {  //j = y
            char cell = passed[j][i];
            if(isalpha(cell)) {
                column += scl::Tile(cell, j, 14 - i, 1);
            }
            else {
                column += scl::Tile(' ', j, 14 - i, 1);
            }
        }
        boardCpy.push_back(column);
    }
    board = boardCpy;
}

void scl::VerticalScrabbleVectorizer::build_board_from(const char** passed) {
    board.clear();
    for (int i = 14; i >= 0; --i) {
        scl::Tstring column;
        for (int j = 0; j < 15; j++) {
            char cell = passed[j][i];
            if(isalpha(cell)) {
                column += scl::Tile(cell, j, 14 - i, 1);
            }
            else {
                column += scl::Tile(' ', j, 14 - i, 1);
            }
        }
        board.push_back(column);
    }
}

void scl::VerticalScrabbleVectorizer::build_perkBoard_from(const char** passed) {
    for (int i = 14; i >= 0; --i) {
        for (int j = 0; j < 15; j++) {
            perkBoard[14 - i][j] = passed[j][i];
        }
    }
}

void scl::VerticalScrabbleVectorizer::guided_place(int x, int y, scl::TYPE type, const scl::Tstring & tstr) {
    if(type == HORIZONTAL){
        for (int i = 0; i < tstr.length(); ++i) {
            board[14 - x - i][y] = tstr.read_at(i);
            board[14 - x - i][y].flag = 1;
        }
    }
    else if(type == VERTICAL){
        for (int i = 0; i < tstr.length(); ++i) {
            board[14 - x][y + i] = tstr.read_at(i);
            board[14 - x][y + i].flag = 1;
        }
    }
    else
        throw std::invalid_argument("Error in \"void scl::VerticalScrabbleVectorizer::guided_place(int x, int y, scl::TYPE type, const scl::Tstring & tstr)\" | Incorrect scl::TYPE passed. scl::TYPE(UNDEFINED_TYPE)");
}

void scl::VerticalScrabbleVectorizer::guided_place(int x, int y, scl::TYPE type, const std::string & str) {
    if(type == HORIZONTAL){
        for (int i = 0; i < str.length(); ++i) {
            board[14 - x - i][y] = str.at(i);
            board[14 - x - i][y].flag = 1;
        }
    }
    else if(type == VERTICAL){
        for (int i = 0; i < str.length(); ++i) {
            board[14 - x][y + i] = str.at(i);
            board[14 - x][y + i].flag = 1;
        }
    }
    else
        throw std::invalid_argument("Error in \"void scl::VerticalScrabbleVectorizer::guided_place(int x, int y, scl::TYPE type, const std::string & str)\" | Incorrect scl::TYPE passed. scl::TYPE(UNDEFINED_TYPE)");
}

void scl::VerticalScrabbleVectorizer::place_best_move_into_board() {
    for (int i = bestWord.read_at(0).x; i < bestWord.length() + bestWord.read_at(0).x; i++) {
        if (board[bestWord.read_at(0).y][i] == ' ')
            board[bestWord.read_at(0).y][i] = scl::Tile(bestWord.read_at(i - bestWord.read_at(0).x).letter,
                                                        i,
                                                        bestWord.read_at(0).y, 1);
        perkBoard[bestWord.read_at(0).y][i] = ' ';
    }
}