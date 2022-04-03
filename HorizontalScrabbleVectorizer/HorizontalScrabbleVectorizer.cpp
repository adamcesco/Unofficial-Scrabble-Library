#include "HorizontalScrabbleVectorizer.h"
#include <sstream>

void HorizontalScrabbleVectorizer::build_board_from(const char* filePath) {
    ifstream boardFile;
    boardFile.open(filePath);
    if(!boardFile.is_open())
        throw invalid_argument("could not open file path passed to void HorizontalScrabbleVectorizer::build_board_from(const char* filePath)");

    string row;
    board.clear();
    int rowCount = 0;
    while (boardFile.good()){
        getline(boardFile, row);
        string cell;
        stringstream strStr(row);
        int cellCount = 0;
        TString LRow;
        while (getline(strStr, cell, ',') && cellCount < 15){
            if(!cell.empty() && isalpha(cell[0])) {
                LRow += Tile(cell[0], cellCount, rowCount, 1);
            }
            else {
                LRow += Tile(' ', cellCount, rowCount, 1);
            }
            cellCount++;
        }
        board.push_back(LRow);
        rowCount++;
    }
    boardFile.close();
}

void HorizontalScrabbleVectorizer::console_print_formatted_board() const{
    for (const auto &row: board) {
        for (int i = 0; i < row.length(); ++i) {
            cout << row.read_at(i).letter;
        }
        cout << endl;
    }
}

string HorizontalScrabbleVectorizer::to_string() const {
    string buffer = "Hand: " + rack + "\n";
    buffer += "\nBest Horizontal Word: " + bestWord.to_string() + " - " + ::to_string(points_of_best_word());
    buffer += "\n\tPostion X: " + ::to_string(bestX);
    buffer += "\n\tPostion Y: " + ::to_string(bestY);
    buffer += "\n\tHorizontal";

    return buffer;
}

void HorizontalScrabbleVectorizer::validate_words() {
    for (int i = 0; i < 15; ++i) {
        for (int j = 0; j < 15; ++j) {
            for (auto& word: moveSets[i][j]) {
                vector<TString> boardCpy = return_raw_board_with(word);

                for (int k = 0; k < 15; k++) {
                    TString row;
                    TString column;
                    for (int l = 0; l < 15; ++l) {
                        row += boardCpy[k].read_at(l);
                        column += boardCpy[l].read_at(k);
                    }

                    vector<string> colShards = column.string_fragments();

                    for (const auto& shard : colShards) {
                        if(shard.length() > 1 && dictionary.find(shard) == dictionary.end()) {
                            word.clear();
                        }
                    }

                    vector<string> rowShards = row.string_fragments();

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

TString HorizontalScrabbleVectorizer::update_best_word() {
    bestWord.clear();
    for (int i = 0; i < 15; ++i) {
        for (int j = 0; j < 15; ++j) {
            for (const auto& word: moveSets[i][j]) {
                int wordPoints = points_of_placed_word(word);
                int bestWordPoints = points_of_placed_word(bestWord);

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

void HorizontalScrabbleVectorizer::validate_board() const{
    if(dictionary.empty())
        throw invalid_argument("Error in void HorizontalScrabbleVectorizer::validate_board() const | unordered_map<TString> dictionary is empty.");

    for (int i = 0; i < 15; ++i) {
        TString row;
        for (int j = 0; j < 15; ++j) {
            row += board[i].read_at(j);
        }

        TString column;
        for (int j = 0; j < 15; ++j) {
            column += board[j].read_at(i);
        }

        vector<string> colShards = column.string_fragments();

        for (const auto& shard : colShards) {
            if(shard.length() > 1 && dictionary.find(shard) == dictionary.end()) {
                throw invalid_argument("Error in void HorizontalScrabbleVectorizer::validate_board() const | Invalid vertical Word in Data/Board.csv |" + shard + '/');
            }
        }

        vector<string> rowShards = row.string_fragments();

        for (const auto& shard : rowShards) {
            if(shard.length() > 1 && dictionary.find(shard) == dictionary.end())
                throw invalid_argument("Error in void HorizontalScrabbleVectorizer::validate_board() const | Invalid horizontal Word in Data/Board.csv |" + shard + '/');
        }
    }
}

vector<string> HorizontalScrabbleVectorizer::return_formatted_perkBoard_copy() const {
    vector<string> toReturn;
    for (int i = 0; i < 15; ++i) {
        string column;
        for (int j = 0; j < 15; ++j) {
            column += perkBoard[i][j];
        }
        toReturn.push_back(column);
    }
    return toReturn;
}

vector<string> HorizontalScrabbleVectorizer::return_formatted_char_board_copy() const {
    vector<string> boardCpy;
    for (int i = 0; i < 15; ++i) {
        string column;
        for (int j = 0; j < 15; ++j) {
            column += board[i].read_at(j).letter;
        }
        boardCpy.push_back(column);
    }
    return boardCpy;
}

void HorizontalScrabbleVectorizer::set_board(const vector<string> &passed) {
    if(passed.size() != 15)
        throw invalid_argument("Error in HorizontalScrabbleVectorizer::set_board(vector<string> passed) | passed argument is not of a proper size.");
    vector<TString> boardCpy;
    for (int i = 0; i < 15; ++i) {
        if(passed[i].length() != 15)
            throw invalid_argument("Error in HorizontalScrabbleVectorizer::set_board(vector<string> passed) | passed argument has an element that is not of a proper size.");

        TString row;
        for (int j = 0; j < 15; ++j) {
            char cell = passed[i][j];
            if(isalpha(cell)) {
                row += Tile(cell, j, i, 1);
            }
            else {
                row += Tile(' ', j, i, 1);
            }
        }
        boardCpy.emplace_back(row);
    }
    board = boardCpy;
}

void HorizontalScrabbleVectorizer::set_perkBoard(const vector<string> &passed) {
    if(passed.size() != 15)
        throw invalid_argument("Error in HorizontalScrabbleVectorizer::set_perkBoard(vector<string> passed) | passed argument is not of a proper size.");

    for (int i = 0; i < 15; ++i) {
        if(passed[i].length() != 15)
            throw invalid_argument("Error in HorizontalScrabbleVectorizer::set_perkBoard(vector<string> passed) | passed argument has an element that is not of a proper size.");
        for (int j = 0; j < 15; ++j) {
            perkBoard[i][j] = passed[i][j];
        }
    }
}

void HorizontalScrabbleVectorizer::set_board(const char** passed) {
    vector<TString> boardCpy;
    for (int i = 0; i < 15; ++i) {
        TString row;
        for (int j = 0; j < 15; ++j) {
            char cell = passed[i][j];
            if(isalpha(cell)) {
                row += Tile(cell, j, i, 1);
            }
            else {
                row += Tile(' ', j, i, 1);
            }
        }
        boardCpy.emplace_back(row);
    }
    board = boardCpy;
}

void HorizontalScrabbleVectorizer::set_perkBoard(const char** passed) {
    for (int i = 0; i < 15; ++i) {
        for (int j = 0; j < 15; ++j) {
            perkBoard[i][j] = passed[i][j];
        }
    }
}
