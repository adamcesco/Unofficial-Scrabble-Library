//
// Created by misc1 on 2/26/2022.
//

#include "HorizontalBoardReader.h"
#include <sstream>

HorizontalBoardReader::HorizontalBoardReader() {
    bestX = bestY = 8;

    ifstream englishWords;
    englishWords.open("../Data/englishWords.txt");
    if(!englishWords.is_open())
        throw invalid_argument("could not open ../Data/englishWords.txt");

    string curWord;
    while(!englishWords.eof()){
        englishWords >> curWord;
        answerSet.emplace(LString(curWord));
    }
    englishWords.close();
}

HorizontalBoardReader::HorizontalBoardReader(const LString& passed) {
    hand = passed;
    bestX = bestY = 8;

    ifstream englishWords;
    englishWords.open("../Data/englishWords.txt");
    if(!englishWords.is_open())
        throw invalid_argument("could not open ../Data/englishWords.txt");

    string curWord;
    while(!englishWords.eof()){
        englishWords >> curWord;
        answerSet.emplace(LString(curWord));
    }
    englishWords.close();
}

void HorizontalBoardReader::build_board() {
    ifstream boardFile;
    boardFile.open("../Data/Board.csv");
    if(!boardFile.is_open())
        throw invalid_argument("could not open ../Data/Board.csv");

    string row;
    int rowCount = 0;
    while (getline(boardFile, row)){
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

    //build rotated board
}

void HorizontalBoardReader::print_board() const{
    for (const auto &row: board) {
        for (int i = 0; i < row.length(); ++i) {
            cout << row.read_at(i).LData;
        }
        cout << endl;
    }
}

string HorizontalBoardReader::to_string() const {
    string buffer = "Hand: " + hand.to_string() + "\n";
    buffer += "\nBest Horizontal Word: " + bestWord.to_string() + " - " + ::to_string(bestWord.get_letter_points() +
                                                                                      perpendicular_points(bestWord));
    buffer += "\n\tPostion X: " + ::to_string(bestX);
    buffer += "\n\tPostion Y: " + ::to_string(bestY);
    buffer += "\n\tHorizontal";

    return buffer;
}

void HorizontalBoardReader::validate_words_perpendicular() {
    for (auto & wordSet : wordSets) {
        for (auto& word: wordSet) {
            vector<LString> boardCpy = return_board_with(word);

            for (int i = 0; i < 15; i++) {  //i = x
                LString column;
                for (int j = 0; j < 15; j++) {  //j = y
                    column += boardCpy[j][i];
                }

                vector<LString> colShards = column.break_into_frags();

                for (const auto& shard : colShards) {
                    if(shard.length() > 1 && answerSet.find(shard) == answerSet.end()){
                        word.clear();
                    }
                }
            }
        }
    }
}

LString HorizontalBoardReader::update_best_word() {
    int rowSubscript = 0;
    bestWord.clear();
    for (auto & wordSet : wordSets) {
        for (const auto& word: wordSet) {
            int wordPoints = word.get_letter_points() + perpendicular_points(word);
            int bestWordPoints = bestWord.get_letter_points() + perpendicular_points(bestWord);

            if (wordPoints > bestWordPoints) {
                bestWord = word;
                bestX = bestWord[0].x + 1;
                bestY = rowSubscript + 1;
            } else if (wordPoints == bestWordPoints) {
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