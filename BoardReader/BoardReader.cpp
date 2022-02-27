//
// Created by misc1 on 2/26/2022.
//

#include "BoardReader.h"
#include <sstream>

BoardReader::BoardReader() {
    bestX = bestY = 8;
    curX =  curY = 8;
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

void BoardReader::traverseBoard() {

}
