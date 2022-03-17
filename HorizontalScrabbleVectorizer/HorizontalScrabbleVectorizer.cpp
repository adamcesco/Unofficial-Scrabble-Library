#include "HorizontalScrabbleVectorizer.h"
#include <sstream>

HorizontalScrabbleVectorizer::HorizontalScrabbleVectorizer() {
    bestX = bestY = 8;

    ifstream englishWords;
    englishWords.open("../Data/scrabble_word_list.txt");
    if(!englishWords.is_open())
        throw invalid_argument("could not open ../Data/scrabble_word_list.txt");

    string curWord;
    while(englishWords.good()){
        getline(englishWords, curWord);
        scrabbleWordSet.emplace(curWord);
    }
    englishWords.close();
}

HorizontalScrabbleVectorizer::HorizontalScrabbleVectorizer(const string &passed) {
    hand = passed;
    bestX = bestY = 8;

    ifstream englishWords;
    englishWords.open("../Data/scrabble_word_list.txt");
    if(!englishWords.is_open())
        throw invalid_argument("could not open ../Data/scrabble_word_list.txt");

    string curWord;
    while(englishWords.good()){
        getline(englishWords, curWord);

        for(auto it: curWord){
            if(!isalpha(it)){
                throw invalid_argument("curWord has a non-alpha in it");
            }
        }

        scrabbleWordSet.emplace(curWord);
    }
    englishWords.close();
}

void HorizontalScrabbleVectorizer::build_board(const string& filePath) {
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
        LString LRow;
        while (getline(strStr, cell, ',') && cellCount < 15){
            if(!cell.empty() && isalpha(cell[0])) {
                LRow += Letter(cell[0], cellCount, rowCount, 1);
            }
            else {
                LRow += Letter(' ', cellCount, rowCount, 1);
            }
            cellCount++;
        }
        board.push_back(LRow);
        rowCount++;
    }
    boardFile.close();

    for (int i = 0; i < 15; ++i) {
        for (int j = 0; j < 15; ++j) {
            if(board[i][j] != ' ')
                perkBoard[i][j] = ' ';
        }
    }
}

void HorizontalScrabbleVectorizer::console_print_formatted_board() const{
    for (const auto &row: board) {
        for (int i = 0; i < row.length(); ++i) {
            cout << row.read_at(i).LData;
        }
        cout << endl;
    }
}

string HorizontalScrabbleVectorizer::to_string() const {
    string buffer = "Hand: " + hand + "\n";
    buffer += "\nBest Horizontal Word: " + bestWord.to_string() + " - " + ::to_string(points_of_best_word());
    buffer += "\n\tPostion X: " + ::to_string(bestX);
    buffer += "\n\tPostion Y: " + ::to_string(bestY);
    buffer += "\n\tHorizontal";

    return buffer;
}

void HorizontalScrabbleVectorizer::validate_words() {
    for (auto & wordSet : answerSets) {
        for (auto& word: wordSet) {
            vector<LString> boardCpy = return_raw_board_with(word);

            for (int i = 0; i < 15; i++) {
                LString row;
                LString column;
                for (int j = 0; j < 15; ++j) {
                    row += boardCpy[i].read_at(j);
                    column += boardCpy[j].read_at(i);
                }

                vector<string> colShards = column.string_fragments();

                for (const auto& shard : colShards) {
                    if(shard.length() > 1 && scrabbleWordSet.find(shard) == scrabbleWordSet.end()) {
                        word.clear();
                    }
                }

                vector<string> rowShards = row.string_fragments();

                for (const auto& shard : rowShards) {
                    if(shard.length() > 1 && scrabbleWordSet.find(shard) == scrabbleWordSet.end()) {
                        word.clear();
                    }
                }
            }
        }
    }
}

LString HorizontalScrabbleVectorizer::update_best_word() {
    int rowSubscript = 0;
    bestWord.clear();
    for (auto & wordSet : answerSets) {
        for (const auto& word: wordSet) {
            int wordPoints = points_of_word(word);
            int bestWordPoints = points_of_word(bestWord);

            if (wordPoints > bestWordPoints) {
                bestWord = word;
                bestX = bestWord[0].x + 1;
                bestY = rowSubscript + 1;
            } else if (wordPoints != 0 && wordPoints == bestWordPoints) {
                if (word.length() < bestWord.length() || bestWord.is_empty()) {
                    bestWord = word;
                    bestX = bestWord[0].x + 1;
                    bestY = rowSubscript + 1;
                }
            }
        }
        rowSubscript++;
    }

    return bestWord;
}

void HorizontalScrabbleVectorizer::validate_board() const{
    if(scrabbleWordSet.empty())
        throw invalid_argument("Error in ScrabbleVectorizer::validate_board() | unordered_map<LString> scrabbleWordSet is empty.");

    for (int i = 0; i < 15; ++i) {
        LString row;
        for (int j = 0; j < 15; ++j) {
            row += board[i].read_at(j);
        }

        LString column;
        for (int j = 0; j < 15; ++j) {
            column += board[j].read_at(i);
        }

        vector<string> colShards = column.string_fragments();

        for (const auto& shard : colShards) {
            if(shard.length() > 1 && scrabbleWordSet.find(shard) == scrabbleWordSet.end())
                throw invalid_argument("Invalid vertical Word in Data/Board.csv |" + shard + '|');
        }

        vector<string> rowShards = row.string_fragments();

        for (const auto& shard : rowShards) {
            if(shard.length() > 1 && scrabbleWordSet.find(shard) == scrabbleWordSet.end())
                throw invalid_argument("Invalid horizontal Word in Data/Board.csv |" + shard + '|');
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
            column += board[i].read_at(j).LData;
        }
        boardCpy.push_back(column);
    }
    return boardCpy;
}

vector<vector<LString>> HorizontalScrabbleVectorizer::return_formatted_answerSets_copy() const {
    vector<vector<LString>> toReturn(15);
    for (const auto & wordSet : answerSets) {
        toReturn.push_back(wordSet);
    }
    return toReturn;
}

void HorizontalScrabbleVectorizer::set_board(const vector<LString> &passed) {
    if(passed.size() != 15)
        throw invalid_argument("Error in HorizontalScrabbleVectorizer::set_board(vector<LString> passed) | passed argument is not of a proper size.");
    vector<LString> boardCpy;
    for (int i = 0; i < 15; ++i) {
        if(passed[i].length() != 15)
            throw invalid_argument("Error in HorizontalScrabbleVectorizer::set_board(vector<LString> passed) | passed argument has an element that is not of a proper size.");

        LString row;
        for (int j = 0; j < 15; ++j) {
            char cell = passed[i].read_at(j).LData;
            if(isalpha(cell)) {
                row += Letter(cell, j, i, 1);
            }
            else {
                row += Letter(' ', j, i, 1);
            }
        }
        boardCpy.emplace_back(row);
    }
    board = boardCpy;

    for (int i = 0; i < 15; ++i) {
        for (int j = 0; j < 15; ++j) {
            if(board[i][j] != ' ')
                perkBoard[i][j] = ' ';
        }
    }
}

void HorizontalScrabbleVectorizer::set_board(const vector<string> &passed) {
    if(passed.size() != 15)
        throw invalid_argument("Error in HorizontalScrabbleVectorizer::set_board(vector<string> passed) | passed argument is not of a proper size.");
    vector<LString> boardCpy;
    for (int i = 0; i < 15; ++i) {
        if(passed[i].length() != 15)
            throw invalid_argument("Error in HorizontalScrabbleVectorizer::set_board(vector<string> passed) | passed argument has an element that is not of a proper size.");

        LString row;
        for (int j = 0; j < 15; ++j) {
            char cell = passed[i][j];
            if(isalpha(cell)) {
                row += Letter(cell, j, i, 1);
            }
            else {
                row += Letter(' ', j, i, 1);
            }
        }
        boardCpy.emplace_back(row);
    }
    board = boardCpy;

    for (int i = 0; i < 15; ++i) {
        for (int j = 0; j < 15; ++j) {
            if(board[i][j] != ' ')
                perkBoard[i][j] = ' ';
        }
    }
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

    for (int i = 0; i < 15; ++i) {
        for (int j = 0; j < 15; ++j) {
            if(board[i][j] != ' ')
                perkBoard[i][j] = ' ';
        }
    }
}
