#include "VerticalScrabbleVectorizer.h"
#include <sstream>

void ssl::VerticalScrabbleVectorizer::build_board_from(const char* filePath) {
    std::ifstream boardFile;
    boardFile.open(filePath);
    if(!boardFile.is_open())
        throw std::invalid_argument("could not open file path passed to void ssl::VerticalScrabbleVectorizer::build_board_from(const char* filePath)");

    std::string row;
    board.clear();
    int rowCount = 0;
    while (boardFile.good()){
        getline(boardFile, row);
        std::string cell;
        std::stringstream strStr(row);
        int cellCount = 0;
        ssl::Tstring LRow;
        while (getline(strStr, cell, ',') && cellCount < 15){
            if(!cell.empty() && isalpha(cell[0])) {
                LRow += ssl::Tile(cell[0], cellCount, rowCount, 1);
            }
            else {
                LRow += ssl::Tile(' ', cellCount, rowCount, 1);
            }
            cellCount++;
        }
        board.push_back(LRow);
        rowCount++;
    }
    boardFile.close();

    std::vector<ssl::Tstring> boardCpy;
    for (int i = 14; i >= 0; i--) {  //i = x
        ssl::Tstring column;
        for (int j = 0; j < 15; j++) {  //j = y
            column += ssl::Tile(board[j][i].letter, j, 14 - i, 1);
        }
        boardCpy.push_back(column);
    }
    board = boardCpy;
}

void ssl::VerticalScrabbleVectorizer::console_print_formatted_board() const{
    for (int i = 0; i < 15; ++i) {
        for (int j = 14; j >= 0; --j) {
            std::cout << board[j].read_at(i).letter;
        }
        std::cout << std::endl;
    }
}

std::string ssl::VerticalScrabbleVectorizer::to_string() const {
    std::string buffer = "Hand: " + rack + "\n";
    buffer += "Best Vertical Word: " + bestWord.to_string() + " - " + std::to_string(points_of_best_word());
    buffer += "\n\tPostion X: " + std::to_string(bestX);
    buffer += "\n\tPostion Y: " + std::to_string(bestY);
    buffer += "\n\tVertical";

    return buffer;
}

ssl::Tstring ssl::VerticalScrabbleVectorizer::update_best_word(){
//    int rowSubscript = 0;
    bestWord.clear();
    for (int i = 0; i < 15; ++i) {
        for (int j = 0; j < 15; ++j) {
            for (const auto& word: moveSets[i][j]) {
                int wordPoints = points_of_placed_word(word) + points_of_placed_word(word);
                int bestWordPoints = points_of_placed_word(bestWord) + points_of_placed_word(bestWord);

                if (wordPoints > bestWordPoints) {
                    bestWord = word;
                    //x = y, y = 14 - x
                    bestX = (14 - i) + 1;
                    bestY = j + 1;
                } else if (wordPoints != 0 && wordPoints == bestWordPoints) {
                    if (word.length() < bestWord.length() || bestWord.is_empty()) {
                        bestWord = word;
                        //x = y, y = 14 - x
                        bestX = (14 - i) + 1;
                        bestY = j + 1;
                    }
                }
            }
        }
    }

    return bestWord;
}

void ssl::VerticalScrabbleVectorizer::validate_words() {
    for (int i = 0; i < 15; ++i) {
        for (int j = 0; j < 15; ++j) {
            for (auto& word: moveSets[i][j]) {
                std::vector<ssl::Tstring> boardCpy = return_raw_board_with(word);

                for (int k = 0; k < 15; k++) {
                    ssl::Tstring row;
                    ssl::Tstring column;
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

void ssl::VerticalScrabbleVectorizer::validate_board() const{
    if(dictionary.empty())
        throw std::invalid_argument("Error in ScrabbleVectorizer::validate_board() | unordered_map<ssl::Tstring> dictionary is empty.");

    for (int i = 0; i < 15; ++i) {
        ssl::Tstring column;
        for (int j = 0; j < 15; ++j) {
            column += board[i].read_at(j);
        }

        ssl::Tstring row;
        for (int j = 14; j >= 0; --j) {
            row += board[j].read_at(i);
        }

        std::vector<std::string> colShards = column.string_fragments();

        for (const auto& shard : colShards) {
            if(shard.length() > 1 && dictionary.find(shard) == dictionary.end())
                throw std::invalid_argument("Error in void ssl::VerticalScrabbleVectorizer::validate_board() | Invalid vertical Word in Data/Board.csv |" + shard + '|');
        }

        std::vector<std::string> rowShards = row.string_fragments();

        for (const auto& shard : rowShards) {
            if(shard.length() > 1 && dictionary.find(shard) == dictionary.end())
                throw std::invalid_argument("Error in void ssl::VerticalScrabbleVectorizer::validate_board() | Invalid horizontal Word in Data/Board.csv |" + shard + '|');
        }
    }
}

std::vector<std::string> ssl::VerticalScrabbleVectorizer::return_formatted_perkBoard_copy() const {
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

std::vector<std::string> ssl::VerticalScrabbleVectorizer::return_formatted_char_board_copy() const {
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

std::vector<ssl::Tstring> ssl::VerticalScrabbleVectorizer::return_formatted_board_copy() const {
    std::vector<ssl::Tstring> boardCpy;
    for (int i = 0; i < 15; ++i) {
        ssl::Tstring column;
        for (int j = 14; j >= 0; --j)
            column += board[j].read_at(i);
        boardCpy.push_back(column);
    }
    return boardCpy;
}

void ssl::VerticalScrabbleVectorizer::set_board(const std::vector<std::string> &passed) {
    if(passed.size() != 15)
        throw std::invalid_argument("Error in ssl::VerticalScrabbleVectorizer::build_board_from(std::vector<std::string> passed) | passed argument is not of a proper size.");

    std::vector<ssl::Tstring> boardCpy;
    for (int i = 14; i >= 0; --i) {  //i = x
        if(passed[i].length() != 15)
            throw std::invalid_argument("Error in ssl::VerticalScrabbleVectorizer::build_board_from(std::vector<std::string> passed) | passed argument has an element that is not of a proper size.");
        ssl::Tstring column;
        for (int j = 0; j < 15; j++) {  //j = y
            char cell = passed[j][i];
            if(isalpha(cell)) {
                column += ssl::Tile(cell, j, 14 - i, 1);
            }
            else {
                column += ssl::Tile(' ', j, 14 - i, 1);
            }
        }
        boardCpy.push_back(column);
    }
    board = boardCpy;
}

void ssl::VerticalScrabbleVectorizer::build_board_from(const char** passed) {
    board.clear();
    for (int i = 14; i >= 0; --i) {
        ssl::Tstring column;
        for (int j = 0; j < 15; j++) {
            char cell = passed[j][i];
            if(isalpha(cell)) {
                column += ssl::Tile(cell, j, 14 - i, 1);
            }
            else {
                column += ssl::Tile(' ', j, 14 - i, 1);
            }
        }
        board.push_back(column);
    }
}

void ssl::VerticalScrabbleVectorizer::build_perkBoard_from(const char** passed) {
    for (int i = 14; i >= 0; --i) {
        for (int j = 0; j < 15; j++) {
            perkBoard[14 - i][j] = passed[j][i];
        }
    }
}
