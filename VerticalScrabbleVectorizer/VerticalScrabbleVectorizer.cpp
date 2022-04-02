#include "VerticalScrabbleVectorizer.h"
#include <sstream>

void VerticalScrabbleVectorizer::build_board_from(const char* filePath) {
    ifstream boardFile;
    boardFile.open(filePath);
    if(!boardFile.is_open())
        throw invalid_argument("could not open file path passed to void VerticalScrabbleVectorizer::build_board_from(const char* filePath)");

    string row;
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

    vector<TString> boardCpy;
    for (int i = 14; i >= 0; i--) {  //i = x
        TString column;
        for (int j = 0; j < 15; j++) {  //j = y
            column += Tile(board[j][i].letter, j, 14 - i, 1);
        }
        boardCpy.push_back(column);
    }
    board = boardCpy;

    for (int i = 0; i < 15; ++i) {
        for (int j = 0; j < 15; ++j) {
            if(board[i][j] != ' ')
                perkBoard[i][j] = ' ';
        }
    }
}

void VerticalScrabbleVectorizer::console_print_formatted_board() const{
    for (int i = 0; i < 15; ++i) {
        for (int j = 14; j >= 0; --j) {
            cout << board[j].read_at(i).letter;
        }
        cout << endl;
    }
}

string VerticalScrabbleVectorizer::to_string() const {
    string buffer = "Hand: " + rack + "\n";
    buffer += "Best Vertical Word: " + bestWord.to_string() + " - " + ::to_string(points_of_best_word());
    buffer += "\n\tPostion X: " + ::to_string(bestX);
    buffer += "\n\tPostion Y: " + ::to_string(bestY);
    buffer += "\n\tVertical";

    return buffer;
}

TString VerticalScrabbleVectorizer::update_best_word(){
//    int rowSubscript = 0;
    bestWord.clear();
    for (int i = 0; i < 15; ++i) {
        for (int j = 0; j < 15; ++j) {
            for (const auto& word: moveSets[i][j]) {
                int wordPoints = points_of_word(word) + points_of_word(word);
                int bestWordPoints = points_of_word(bestWord) + points_of_word(bestWord);

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

void VerticalScrabbleVectorizer::validate_words() {
    for (int i = 0; i < 15; ++i) {
        for (int j = 0; j < 15; ++j) {
            for (auto& word: moveSets[i][j]) {
                vector<TString> boardCpy = return_raw_board_with(word);

                for (int i = 0; i < 15; i++) {
                    TString row;
                    TString column;
                    for (int j = 0; j < 15; ++j) {
                        row += boardCpy[i].read_at(j);
                        column += boardCpy[14 - j].read_at(i);
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

void VerticalScrabbleVectorizer::set_board(const vector<TString> &passed) {   //assumes that passed is formatted as a proper board
    if(passed.size() != 15)
        throw invalid_argument("Error in VerticalScrabbleVectorizer::set_board(vector<TString>) | passed argument is not of a proper size.");

    vector<TString> boardCpy;
    for (int i = 14; i >= 0; i--) {  //i = x
        if(passed[i].length() != 15)
            throw invalid_argument("Error in VerticalScrabbleVectorizer::set_board(vector<TString>) | passed argument has an element that is not of a proper size.");
        TString column;
        for (int j = 0; j < 15; j++) {  //j = y
            char cell = passed[j].read_at(i).letter;
            if(isalpha(cell)) {
                column += Tile(cell, j, 14 - i, 1);
            }
            else {
                column += Tile(' ', j, 14 - i, 1);
            }
        }
        boardCpy.push_back(column);
    }
    board = boardCpy;

    for (int i = 0; i < 15; ++i) {
        for (int j = 0; j < 15; ++j) {
            if(board[i][j] != ' ')
                perkBoard[i][j] = ' ';
        }
    }
}

void VerticalScrabbleVectorizer::validate_board() const{
    if(dictionary.empty())
        throw invalid_argument("Error in ScrabbleVectorizer::validate_board() | unordered_map<TString> dictionary is empty.");

    for (int i = 0; i < 15; ++i) {
        TString column;
        for (int j = 0; j < 15; ++j) {
            column += board[i].read_at(j);
        }

        TString row;
        for (int j = 14; j >= 0; --j) {
            row += board[j].read_at(i);
        }

        vector<string> colShards = column.string_fragments();

        for (const auto& shard : colShards) {
            if(shard.length() > 1 && dictionary.find(shard) == dictionary.end())
                throw invalid_argument("Error in void VerticalScrabbleVectorizer::validate_board() | Invalid vertical Word in Data/Board.csv |" + shard + '|');
        }

        vector<string> rowShards = row.string_fragments();

        for (const auto& shard : rowShards) {
            if(shard.length() > 1 && dictionary.find(shard) == dictionary.end())
                throw invalid_argument("Error in void VerticalScrabbleVectorizer::validate_board() | Invalid horizontal Word in Data/Board.csv |" + shard + '|');
        }
    }
}

vector<string> VerticalScrabbleVectorizer::return_formatted_perkBoard_copy() const {
    vector<string> toReturn;
    for (int i = 0; i < 15; ++i) {
        string column;
        for (int j = 14; j >= 0; --j) {
            column += perkBoard[j][i];
        }
        toReturn.push_back(column);
    }
    return toReturn;
}

vector<string> VerticalScrabbleVectorizer::return_formatted_char_board_copy() const {
    vector<string> boardCpy;
    for (int i = 0; i < 15; ++i) {
        string column;
        for (int j = 14; j >= 0; --j) {
            column += board[j].read_at(i).letter;
        }
        boardCpy.push_back(column);
    }
    return boardCpy;
}

vector<TString> VerticalScrabbleVectorizer::return_formatted_board_copy() const {
    vector<TString> boardCpy;
    for (int i = 0; i < 15; ++i) {
        TString column;
        for (int j = 14; j >= 0; --j)
            column += board[j].read_at(i);
        boardCpy.push_back(column);
    }
    return boardCpy;
}

void VerticalScrabbleVectorizer::set_board(const vector<string> &passed) {
    if(passed.size() != 15)
        throw invalid_argument("Error in VerticalScrabbleVectorizer::set_board(vector<string> passed) | passed argument is not of a proper size.");

    vector<TString> boardCpy;
    for (int i = 14; i >= 0; --i) {  //i = x
        if(passed[i].length() != 15)
            throw invalid_argument("Error in VerticalScrabbleVectorizer::set_board(vector<string> passed) | passed argument has an element that is not of a proper size.");
        TString column;
        for (int j = 0; j < 15; j++) {  //j = y
            char cell = passed[j][i];
            if(isalpha(cell)) {
                column += Tile(cell, j, 14 - i, 1);
            }
            else {
                column += Tile(' ', j, 14 - i, 1);
            }
        }
        boardCpy.push_back(column);
    }
    board = boardCpy;

    for (int i = 0; i < 15; ++i) {
        for (int j = 0; j < 15; ++j) {
            if(board[i][j] != ' ')
                perkBoard[i][j] = ' ';
        }
    }
}

void VerticalScrabbleVectorizer::set_perkBoard(const vector<string> &passed) {
    if(passed.size() != 15)
        throw invalid_argument("Error in VerticalScrabbleVectorizer::set_perkBoard(vector<string> passed) | passed argument is not of a proper size.");

    for (int i = 14; i >= 0; --i) {
        if(passed[i].length() != 15)
            throw invalid_argument("Error in VerticalScrabbleVectorizer::set_perkBoard(vector<string> passed) | passed argument has an element that is not of a proper size.");
        for (int j = 0; j < 15; ++j) {
            perkBoard[14 - i][j] = passed[j][i];
        }
    }

    for (int i = 0; i < 15; ++i) {
        for (int j = 0; j < 15; ++j) {
            if(board[i][j] != ' ')
                perkBoard[i][j] = ' ';
        }
    }
}
