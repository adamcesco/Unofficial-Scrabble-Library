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
        while (getline(strStr, cell, ',')){
            if(cell.size() > 0)
                board[rowCount][cellCount] = cell[0];
            else
                board[rowCount][cellCount] = ' ';
            cellCount++;
        }
        rowCount++;
    }
    boardFile.close();
}

void BoardReader::printBoard() const{
    for (const auto& row : board) {
        for (const auto& cell: row) {
            cout << cell;
        }
        cout << endl;
    }
}

void BoardReader:SearchBoardHorizontal() {
    int rowCount = 0;
    int prev = 0;
    for (const auto& word : board) {
        int k = 0;
        int subScrOfLastSubstr = 0;
        while (k < 15) {
            string subStr = "";
            while(isalpha(word[k]) && k < 15){
                subStr += word[k];
                k++;
                subScrOfLastSubstr = k;
            }
            if(!subStr.empty()){
                HandManager handManager(hand + subStr);
                handManager.SortManager();
                handManager.PowerSet();
                handManager.StartPermute();
                handManager.CleanPossibleAnswers();
                string curBestWord = handManager.GetBestWord(k - prev, subStr,15 - k);
                cout << curBestWord << endl;
                if(HandManager::gradeWord(curBestWord) > HandManager::gradeWord(bestWord)){
                    bestWord = curBestWord;
                    bestX = k;
                    bestY = 15 - rowCount;
                    horizontal = true;
                }
            }
            k++;
        }
        rowCount++;
        prev = subScrOfLastSubstr;
    }
}

string BoardReader::to_string() const {
    string buffer = "Best Word: " + bestWord + " - " + ::to_string(HandManager::gradeWord(bestWord));
    buffer += "\nPostion X: " + ::to_string(bestX);
    buffer += "\nPostion Y: " + ::to_string(bestY);
    if(horizontal)
        buffer += "\nHorizontal";
    else
        buffer += "\nVertical";

    return buffer;
}
