#include "VerticalScrabbleVectorizer.h"
#include <sstream>

VerticalScrabbleVectorizer::VerticalScrabbleVectorizer() {
    ifstream englishWords;
    englishWords.open("../Data/scrabble_word_list.txt");
    if(!englishWords.is_open())
        throw invalid_argument("could not open ../Data/scrabble_word_list.txt");

    string curWord;
    while(englishWords.good()){
        getline(englishWords, curWord);
        scrabbleWordSet.emplace(LString(curWord));
    }
    englishWords.close();
}

VerticalScrabbleVectorizer::VerticalScrabbleVectorizer(const string &passed) {
    hand = passed;

    ifstream englishWords;
    englishWords.open("../Data/scrabble_word_list.txt");
    if(!englishWords.is_open())
        throw invalid_argument("could not open ../Data/scrabble_word_list.txt");

    string curWord;
    while(englishWords.good()){
        getline(englishWords, curWord);
        scrabbleWordSet.emplace(LString(curWord));
    }
    englishWords.close();
}

void VerticalScrabbleVectorizer::build_board(const string& filePath) {
    ifstream boardFile;
    boardFile.open(filePath);
    if(!boardFile.is_open())
        throw invalid_argument("could not open " + filePath);

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
        board.push_back(rowVect);
        rowCount++;
    }
    boardFile.close();

    vector<LString> boardCpy;
    for (int i = 14; i >= 0; i--) {  //i = x
        LString column;
        for (int j = 0; j < 15; j++) {  //j = y
            column += Letter(board[j][i].LData, j, 14 - i, 1);
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
            cout << board[j].read_at(i).LData;
        }
        cout << endl;
    }
}

string VerticalScrabbleVectorizer::to_string() const {
    string buffer = "Hand: " + hand + "\n";
    buffer += "Best Vertical Word: " + bestWord.to_string() + " - " + ::to_string(points_of_best_word());
    buffer += "\n\tPostion X: " + ::to_string(bestX);
    buffer += "\n\tPostion Y: " + ::to_string(bestY);
    buffer += "\n\tVertical";

    return buffer;
}

LString VerticalScrabbleVectorizer::update_best_word(){
    int rowSubscript = 0;
    bestWord.clear();
    for (auto & wordSet : answerSets) {
        for (const auto& word: wordSet) {
            int wordPoints = points_of_word(word) + points_of_word(word);
            int bestWordPoints = points_of_word(bestWord) + points_of_word(bestWord);

            if (wordPoints > bestWordPoints) {
                bestWord = word;
                //x = y, y = 14 - x
                bestX = (14 - rowSubscript) + 1;
                bestY = (bestWord[0].x) + 1;
            } else if (wordPoints != 0 && wordPoints == bestWordPoints) {
                if (word.length() < bestWord.length() || bestWord.is_empty()) {
                    bestWord = word;
                    //x = y, y = 14 - x
                    bestX = (14 - rowSubscript) + 1;
                    bestY = (bestWord[0].x) + 1;
                }
            }
        }
        rowSubscript++;
    }

    return bestWord;
}

void VerticalScrabbleVectorizer::validate_words() {
    for (auto & wordSet : answerSets) {
        for (auto& word: wordSet) {
            vector<LString> boardCpy = return_raw_board_with(word);

            for (int i = 0; i < 15; i++) {
                LString row;
                LString column;
                for (int j = 0; j < 15; ++j) {
                    row += boardCpy[i].read_at(j);
                    column += boardCpy[14 - j].read_at(i);
                }

                vector<LString> colShards = column.fragments();

                for (const auto& shard : colShards) {
                    if(shard.length() > 1 && scrabbleWordSet.find(shard) == scrabbleWordSet.end()) {
                        word.clear();
                    }
                }

                vector<LString> rowShards = row.fragments();

                for (const auto& shard : rowShards) {
                    if(shard.length() > 1 && scrabbleWordSet.find(shard) == scrabbleWordSet.end()) {
                        word.clear();
                    }
                }
            }
        }
    }
}

void VerticalScrabbleVectorizer::set_board(const vector<LString> &passed) {   //assumes that passed is formatted as a proper board
    if(passed.size() != 15)
        throw invalid_argument("Error in VerticalScrabbleVectorizer::set_board(vector<LString> passed) | passed argument is not of a proper size.");

    vector<LString> boardCpy;
    for (int i = 14; i >= 0; i--) {  //i = x
        if(passed[i].length() != 15)
            throw invalid_argument("Error in VerticalScrabbleVectorizer::set_board(vector<LString> passed) | passed argument has an element that is not of a proper size.");
        LString column;
        for (int j = 0; j < 15; j++) {  //j = y
            char cell = passed[j].read_at(i).LData;
            if(isalpha(cell)) {
                column += Letter(cell, j, 14 - i, 1);
            }
            else {
                column += Letter(' ', j, 14 - i, 1);
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
    if(scrabbleWordSet.empty())
        throw invalid_argument("Error in ScrabbleVectorizer::validate_board() | unordered_map<LString> scrabbleWordSet is empty.");

    for (int i = 0; i < 15; ++i) {
        LString column;
        for (int j = 0; j < 15; ++j) {
            column += board[i].read_at(j);
        }

        LString row;
        for (int j = 14; j >= 0; --j) {
            row += board[j].read_at(i);
        }

        vector<LString> colShards = column.fragments();

        for (const auto& shard : colShards) {
            if(shard.length() > 1 && scrabbleWordSet.find(shard) == scrabbleWordSet.end())
                throw invalid_argument("Invalid vertical Word in Data/Board.csv | " + shard.to_string());
        }

        vector<LString> rowShards = row.fragments();

        for (const auto& shard : rowShards) {
            if(shard.length() > 1 && scrabbleWordSet.find(shard) == scrabbleWordSet.end())
                throw invalid_argument("Invalid horizontal Word in Data/Board.csv | " + shard.to_string());
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
            column += board[j].read_at(i).LData;
        }
        boardCpy.push_back(column);
    }
    return boardCpy;
}

vector<vector<LString>> VerticalScrabbleVectorizer::return_formatted_answerSets_copy() const {
    vector<vector<LString>> toReturn;
    for (int i = 0; i < 15; ++i) {
        toReturn.push_back(answerSets[14 - i]);
    }
    return toReturn;
}

vector<LString> VerticalScrabbleVectorizer::return_formatted_board_copy() const {
    vector<LString> boardCpy;
    for (int i = 0; i < 15; ++i) {
        LString column;
        for (int j = 14; j >= 0; --j)
            column += board[j].read_at(i);
        boardCpy.push_back(column);
    }
    return boardCpy;
}

void VerticalScrabbleVectorizer::set_board(const vector<string> &passed) {
    if(passed.size() != 15)
        throw invalid_argument("Error in VerticalScrabbleVectorizer::set_board(vector<string> passed) | passed argument is not of a proper size.");

    vector<LString> boardCpy;
    for (int i = 14; i >= 0; --i) {  //i = x
        if(passed[i].length() != 15)
            throw invalid_argument("Error in VerticalScrabbleVectorizer::set_board(vector<string> passed) | passed argument has an element that is not of a proper size.");
        LString column;
        for (int j = 0; j < 15; j++) {  //j = y
            char cell = passed[j][i];
            if(isalpha(cell)) {
                column += Letter(cell, j, 14 - i, 1);
            }
            else {
                column += Letter(' ', j, 14 - i, 1);
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
