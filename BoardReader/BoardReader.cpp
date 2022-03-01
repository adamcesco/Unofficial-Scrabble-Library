//
// Created by misc1 on 2/26/2022.
//

#include "BoardReader.h"
#include <sstream>

BoardReader::BoardReader() {
    bestX = bestY = 8;
    curX =  curY = 8;
    bestWord = "";
}

BoardReader::BoardReader(string passed) {
    hand = passed;
    bestX = bestY = 8;
    curX =  curY = 8;
    bestWord = "";
}

void BoardReader::buildBoard() {
    ifstream boardFile;
    boardFile.open("../data/Board.csv");
    if(!boardFile.is_open())
        throw invalid_argument("could not open ../data/englishWords.txt");

    string row;
    int rowCount = 0;
    while (getline(boardFile, row)){
        string cell;
        stringstream strStr(row);
        int cellCount = 0;
        LString rowVect;
        while (getline(strStr, cell, ',')){
            if(cell.size() > 0 && isalpha(cell[0])) {
                rowVect.push_back(Letter(cell[0], cellCount, rowCount));
            }
            else {
                rowVect.push_back(Letter(' ', cellCount, rowCount));
            }
            cellCount++;
        }
        board.push_back(rowVect);
        rowCount++;
    }
    boardFile.close();
}

void BoardReader::printBoard() const{
    for (const auto& row : board) {
        for (int i = 0; i < row.size(); ++i) {
            cout << row.read_at(i).LData;
        }
        cout << endl;
    }
}

void BoardReader::SearchBoardHorizontal() {
    ifstream englishWords;
    englishWords.open("../data/englishWords.txt");
    if(!englishWords.is_open())
        throw invalid_argument("could not open ../data/englishWords.txt");

    vector<LString> answers;
    string curWord;
    while(!englishWords.eof()){
        englishWords >> curWord;
        answers.emplace_back(LString(curWord));
    }
    englishWords.close();


    int rowCount = 1;
    for (auto& row : board) {
        for (auto& word: answers) {
            if(word.containsIgnorePadding(row) && word.isDescendentOf(hand + row)) {
                if(HandManager::gradeWord(word.to_string()) > HandManager::gradeWord(bestWord)){
                    bestWord = word.to_string();
                    bestX = -1;
                    bestY = rowCount;
                    horizontal = true;
                }
                else if(HandManager::gradeWord(word.to_string()) == HandManager::gradeWord(bestWord)){
                    if(word.size() < bestWord.length() || bestWord.empty()){
                        bestWord = word.to_string();
                        bestX = -1;
                        bestY = rowCount;
                        horizontal = true;
                    }
                }
            }
        }
        rowCount++;
    }
}

string BoardReader::to_string() const {
    string buffer = "Hand: " + hand.to_string() + "\n";
    buffer += "Best Word: " + bestWord + " - " + ::to_string(HandManager::gradeWord(bestWord));
    buffer += "\nPostion X: " + ::to_string(bestX);
    buffer += "\nPostion Y: " + ::to_string(bestY);
    if(horizontal)
        buffer += "\nHorizontal";
    else
        buffer += "\nVertical";

    return buffer;
}
