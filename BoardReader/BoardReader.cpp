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
            if(cell.size() > 0) {
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
    int rowCount = 1;
    int prev = 0;
    for (auto& row : board) {
        HandManager handManager(hand);
        handManager.SortManager();
        handManager.PowerSet();
        handManager.StartPermute();
        handManager.CleanPossibleAnswers();
        string curBestWord = handManager.GetBestWord(row);

//        cout << curBestWord << endl << endl;
        if(HandManager::gradeWord(curBestWord) >= HandManager::gradeWord(bestWord)){
            if(curBestWord.length() < bestWord.length() || bestWord.empty()){
                bestWord = curBestWord;
                bestX = -1;
                bestY = rowCount;
                horizontal = true;
            }
        }
        rowCount++;
    }
}

//void BoardReader::SearchBoardHorizontal() {
//    int rowCount = 1;
//    int prev = 0;
//    for (const auto& row : board) {
//        int k = 0;
//        int subScrOfLastSubstr = 0;
//        while (k < 15) {
//            string subStr = "";
//            while(isalpha(row[k].data) && k < 15){
//                subStr += row[k].data;
//                k++;
//                subScrOfLastSubstr = k;
//            }
//            if(!subStr.empty()){
//                HandManager handManager(hand);
//                handManager.SortManager();
//                handManager.PowerSet();
//                handManager.StartPermute();
//                handManager.CleanPossibleAnswers();
//                string curBestWord = handManager.GetBestWord((k - prev) - subStr.length(), subStr,15 - k);
//                if(HandManager::gradeWord(curBestWord) >= HandManager::gradeWord(bestWord)){
//                    if(curBestWord.length() < bestWord.length() || bestWord.empty()){
//                        bestWord = curBestWord;
//                        bestX = k;
//                        bestY = rowCount;
//                        horizontal = true;
//                    }
//                }
//            }
//            k++;
//            prev = subScrOfLastSubstr;
//        }
//        rowCount++;
//    }
//}

string BoardReader::to_string() const {
    string buffer = "Hand: " + hand + "\n";
    buffer += "Best Word: " + bestWord + " - " + ::to_string(HandManager::gradeWord(bestWord));
    buffer += "\nPostion X: " + ::to_string(bestX);
    buffer += "\nPostion Y: " + ::to_string(bestY);
    if(horizontal)
        buffer += "\nHorizontal";
    else
        buffer += "\nVertical";

    return buffer;
}
