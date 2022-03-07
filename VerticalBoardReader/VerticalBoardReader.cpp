//
// Created by misc1 on 3/3/2022.
//

#include "VerticalBoardReader.h"
#include <sstream>

VerticalBoardReader::VerticalBoardReader() {
    ifstream englishWords;
    englishWords.open("../Data/scrabble_word_list.txt");
    if(!englishWords.is_open())
        throw invalid_argument("could not open ../Data/scrabble_word_list.txt");

    string curWord;
    while(englishWords.good()){
        getline(englishWords, curWord);
        answerSet.emplace(LString(curWord));
    }
    englishWords.close();
}

VerticalBoardReader::VerticalBoardReader(const LString& passed) {
    hand = passed;

    ifstream englishWords;
    englishWords.open("../Data/scrabble_word_list.txt");
    if(!englishWords.is_open())
        throw invalid_argument("could not open ../Data/scrabble_word_list.txt");

    string curWord;
    while(englishWords.good()){
        getline(englishWords, curWord);
        answerSet.emplace(LString(curWord));
    }
    englishWords.close();
}

void VerticalBoardReader::build_board() {
    ifstream boardFile;
    boardFile.open("../Data/Board.csv");
    if(!boardFile.is_open())
        throw invalid_argument("could not open ../Data/Board.csv");

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
}

void VerticalBoardReader::print_foramtted_board() const{
    for (int i = 0; i < 15; ++i) {
        for (int j = 14; j >= 0; --j) {
            cout << board[j].read_at(i).LData;
        }
        cout << endl;
    }
}

string VerticalBoardReader::to_string() const {
    string buffer = "Hand: " + hand.to_string() + "\n";
    buffer += "Best Vertical Word: " + bestWord.to_string() + " - " + ::to_string(bestWord.get_letter_points() +
                                                                                   perpendicular_points(bestWord));
    buffer += "\n\tPostion X: " + ::to_string(bestX);
    buffer += "\n\tPostion Y: " + ::to_string(bestY);
    buffer += "\n\tVertical";

    return buffer;
}

LString VerticalBoardReader::update_best_word(){
    int rowSubscript = 0;
    bestWord.clear();
    for (auto & wordSet : wordSets) {
        for (const auto& word: wordSet) {
            int wordPoints = word.get_letter_points() + perpendicular_points(word);
            int bestWordPoints = bestWord.get_letter_points() + perpendicular_points(bestWord);

            if (wordPoints > bestWordPoints) {
                bestWord = word;
                //x = y, y = 14 - x
                bestX = (14 - rowSubscript) + 1;
                bestY = (bestWord[0].x) + 1;
            } else if (wordPoints == bestWordPoints) {
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

void VerticalBoardReader::validate_words() {
    for (auto & wordSet : wordSets) {
        for (auto& word: wordSet) {
            vector<LString> boardCpy = return_raw_board_with(word);

            for (int i = 0; i < 15; i++) {
                LString row;
                LString column;
                for (int j = 0; j < 15; ++j) {
                    row += boardCpy[i].read_at(j);
                    column += boardCpy[14 - j].read_at(i);
                }

                vector<LString> colShards = column.break_into_frags();

                for (const auto& shard : colShards) {
                    if(shard.length() > 1 && answerSet.find(shard) == answerSet.end()) {
                        word.clear();
                    }
                }

                vector<LString> rowShards = row.break_into_frags();

                for (const auto& shard : rowShards) {
                    if(shard.length() > 1 && answerSet.find(shard) == answerSet.end()) {
                        word.clear();
                    }
                }
            }
        }
    }
}

void VerticalBoardReader::set_board(vector<LString> passed) {
    vector<LString> boardCpy;
    for (int i = 14; i >= 0; i--) {  //i = x
        LString column;
        for (int j = 0; j < 15; j++) {  //j = y
            column += Letter(passed[j][i].LData, j, 14 - i, 1);
        }
        boardCpy.push_back(column);
    }
    board = boardCpy;
}

void VerticalBoardReader::validate_board() const{
    if(answerSet.empty())
        throw invalid_argument("Error in ScrabbleReader::validate_board() | unordered_map<LString> answerSet is empty.");

    for (int i = 0; i < 15; ++i) {
        LString column;
        for (int j = 0; j < 15; ++j) {
            column += board[i].read_at(j);
        }

        LString row;
        for (int j = 14; j >= 0; --j) {
            row += board[j].read_at(i);
        }

        vector<LString> colShards = column.break_into_frags();

        for (const auto& shard : colShards) {
            if(shard.length() > 1 && answerSet.find(shard) == answerSet.end())
                throw invalid_argument("Invalid vertical Word in Data/Board.csv | " + shard.to_string());
        }

        vector<LString> rowShards = row.break_into_frags();

        for (const auto& shard : rowShards) {
            if(shard.length() > 1 && answerSet.find(shard) == answerSet.end())
                throw invalid_argument("Invalid horizontal Word in Data/Board.csv | " + shard.to_string());
        }
    }
}