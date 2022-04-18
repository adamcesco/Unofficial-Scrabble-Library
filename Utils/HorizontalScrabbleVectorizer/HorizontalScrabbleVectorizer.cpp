#include "HorizontalScrabbleVectorizer.h"
#include <sstream>
#include <vector>
#include <iostream>
#include <fstream>

void scl::HorizontalScrabbleVectorizer::build_board_from(const char* filePath) {
    std::ifstream boardFile;
    boardFile.open(filePath);
    if(!boardFile.is_open())
        throw std::invalid_argument("could not open file path passed to void scl::HorizontalScrabbleVectorizer::build_board_from(const char* filePath)");

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
}

void scl::HorizontalScrabbleVectorizer::console_print_formatted_board() const{
    for (const auto &row: board) {
        for (int i = 0; i < row.length(); ++i) {
            std::cout << row.read_at(i).letter;
        }
        std::cout << std::endl;
    }
}

std::string scl::HorizontalScrabbleVectorizer::to_string() const {
    std::string buffer = "Hand: " + rack + "\n";
    buffer += "\nBest Horizontal Word: " + bestWord.to_string() + " - " + std::to_string(points_of_best_boarded_move());
    buffer += "\n\tPostion X: " + std::to_string(bestX);
    buffer += "\n\tPostion Y: " + std::to_string(bestY);
    buffer += "\n\tHorizontal";

    return buffer;
}

void scl::HorizontalScrabbleVectorizer::validate_generated_moves() {
    for (int i = 0; i < 15; ++i) {
        for (int j = 0; j < 15; ++j) {
            for (auto& word: moveSets[i][j]) {
                std::vector<scl::Tstring> boardCpy = return_raw_board_with(word);

                for (int k = 0; k < 15; k++) {
                    scl::Tstring row;
                    scl::Tstring column;
                    for (int l = 0; l < 15; ++l) {
                        row += boardCpy[k].read_at(l);
                        column += boardCpy[l].read_at(k);
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

scl::Tstring scl::HorizontalScrabbleVectorizer::update_best_move() {
    bestWord.clear();
    for (int i = 0; i < 15; ++i) {
        for (int j = 0; j < 15; ++j) {
            for (const auto& word: moveSets[i][j]) {
                int wordPoints = points_of_raw_boarded_tstr(word);
                int bestWordPoints = points_of_raw_boarded_tstr(bestWord);

                if (wordPoints > bestWordPoints) {
                    bestWord = word;
                    bestX = bestWord[0].x + 1;
                    bestY = i + 1;
                } else if (wordPoints != 0 && wordPoints == bestWordPoints) {
                    if (word.length() < bestWord.length() || bestWord.is_empty()) {
                        bestWord = word;
                        bestX = bestWord[0].x + 1;
                        bestY = i + 1;
                    }
                }
            }
        }
    }

    return bestWord;
}

void scl::HorizontalScrabbleVectorizer::validate_board() const{
    if(dictionary.empty())
        throw std::invalid_argument("Error in void scl::HorizontalScrabbleVectorizer::validate_board() const | unordered_map<scl::Tstring> dictionary is empty.");

    for (int i = 0; i < 15; ++i) {
        scl::Tstring row;
        for (int j = 0; j < 15; ++j) {
            row += board[i].read_at(j);
        }

        scl::Tstring column;
        for (int j = 0; j < 15; ++j) {
            column += board[j].read_at(i);
        }

        std::vector<std::string> colShards = column.string_fragments();

        for (const auto& shard : colShards) {
            if(shard.length() > 1 && dictionary.find(shard) == dictionary.end()) {
                throw std::invalid_argument("Error in void scl::HorizontalScrabbleVectorizer::validate_board() const | Invalid vertical Word in Data/Board.csv |" + shard + '/');
            }
        }

        std::vector<std::string> rowShards = row.string_fragments();

        for (const auto& shard : rowShards) {
            if(shard.length() > 1 && dictionary.find(shard) == dictionary.end())
                throw std::invalid_argument("Error in void scl::HorizontalScrabbleVectorizer::validate_board() const | Invalid horizontal Word in Data/Board.csv |" + shard + '/');
        }
    }
}

std::vector<std::string> scl::HorizontalScrabbleVectorizer::return_formatted_perkBoard_copy() const {
    std::vector<std::string> toReturn;
    for (int i = 0; i < 15; ++i) {
        std::string column;
        for (int j = 0; j < 15; ++j) {
            column += perkBoard[i][j];
        }
        toReturn.push_back(column);
    }
    return toReturn;
}

std::vector<std::string> scl::HorizontalScrabbleVectorizer::return_formatted_char_board_copy() const {
    std::vector<std::string> boardCpy;
    for (int i = 0; i < 15; ++i) {
        std::string column;
        for (int j = 0; j < 15; ++j) {
            column += board[i].read_at(j).letter;
        }
        boardCpy.push_back(column);
    }
    return boardCpy;
}

void scl::HorizontalScrabbleVectorizer::set_board(const std::vector<std::string> &passed) {
    if(passed.size() != 15)
        throw std::invalid_argument("Error in scl::HorizontalScrabbleVectorizer::build_board_from(std::vector<std::string> passed) | passed argument is not of a proper size.");
    std::vector<scl::Tstring> boardCpy;
    for (int i = 0; i < 15; ++i) {
        if(passed[i].length() != 15)
            throw std::invalid_argument("Error in scl::HorizontalScrabbleVectorizer::build_board_from(std::vector<std::string> passed) | passed argument has an element that is not of a proper size.");

        scl::Tstring row;
        for (int j = 0; j < 15; ++j) {
            char cell = passed[i][j];
            if(isalpha(cell)) {
                row += scl::Tile(cell, j, i, 1);
            }
            else {
                row += scl::Tile(' ', j, i, 1);
            }
        }
        boardCpy.emplace_back(row);
    }
    board = boardCpy;
}

void scl::HorizontalScrabbleVectorizer::build_board_from(const char** passed) {
    board.clear();
    for (int i = 0; i < 15; ++i) {
        scl::Tstring row;
        for (int j = 0; j < 15; ++j) {
            char cell = passed[i][j];
            if(isalpha(cell)) {
                row += scl::Tile(cell, j, i, 1);
            }
            else {
                row += scl::Tile(' ', j, i, 1);
            }
        }
        board.emplace_back(row);
    }
}

void scl::HorizontalScrabbleVectorizer::build_perkBoard_from(const char** passed) {
    for (int i = 0; i < 15; ++i) {
        for (int j = 0; j < 15; ++j) {
            perkBoard[i][j] = passed[i][j];
        }
    }
}

void scl::HorizontalScrabbleVectorizer::guided_place(int x, int y, scl::TYPE type, const scl::Tstring &tstr) {
    if(type == HORIZONTAL){
        for (int i = 0; i < tstr.length(); ++i) {
            board[y][x + i] = tstr.read_at(i);
            board[y][x + i].flag = 1;
            perkBoard[y][x + i] = ' ';
        }
    }
    else if(type == VERTICAL){
        for (int i = 0; i < tstr.length(); ++i) {
            board[y + i][x] = tstr.read_at(i);
            board[y + i][x].flag = 1;
            perkBoard[y + i][x] = ' ';
        }
    }
    else
        throw std::invalid_argument("Error in \"void scl::HorizontalScrabbleVectorizer::guided_place(int x, int y, scl::TYPE type, const scl::Tstring & tstr)\" | Incorrect scl::TYPE passed. scl::TYPE(UNDEFINED_TYPE)");
}

void scl::HorizontalScrabbleVectorizer::guided_place(int x, int y, scl::TYPE type, const std::string &str) {
    if(type == HORIZONTAL){
        for (int i = 0; i < str.length(); ++i) {
            board[y][x + i] = str.at(i);
            board[y][x + i].flag = 1;
            perkBoard[y][x + i] = ' ';
        }
    }
    else if(type == VERTICAL){
        for (int i = 0; i < str.length(); ++i) {
            board[y + i][x] = str.at(i);
            board[y + i][x].flag = 1;
            perkBoard[y + i][x] = ' ';
        }
    }
    else
        throw std::invalid_argument("Error in \"void scl::HorizontalScrabbleVectorizer::guided_place(int x, int y, scl::TYPE type, const std::string & str)\" | Incorrect scl::TYPE passed. scl::TYPE(UNDEFINED_TYPE)");
}
