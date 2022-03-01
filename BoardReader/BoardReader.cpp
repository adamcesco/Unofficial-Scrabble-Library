//
// Created by misc1 on 2/26/2022.
//

#include "BoardReader.h"
#include <sstream>

BoardReader::BoardReader() {
    bestX = bestY = 8;
    curX =  curY = 8;
}

BoardReader::BoardReader(LString passed) {
    hand = passed;
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
        LString rowVect;
        while (getline(strStr, cell, ',')){
            if(cell.size() > 0 && isalpha(cell[0])) {
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
}

void BoardReader::printBoard() const{
    for (const auto& row : board) {
        for (int i = 0; i < row.length(); ++i) {
            cout << row.read_at(i).LData;
        }
        cout << endl;
    }
}

bool myComp(LString str1, LString str2){
    int sum1 = 0;
    int sum2 = 0;
    for (int i = 0; i < str1.length(); ++i) {
        sum1 += legend[(str1.read_at(i).LData & 31) - 1];
    }

    for (int i = 0; i < str2.length(); ++i) {
        sum2 += legend[(str2.read_at(i).LData & 31) - 1];
    }

    if(sum1 > sum2) {
        return true;
    }
    else if (sum1 < sum2) {
        return false;
    }
    else if(str1.length() > str2.length()){
        return false;
    }
    else if(str1.length() < str2.length()){
        return true;
    }
    else{
        int str1HighValCount = 0;
        int str2HighValCount = 0;
        for (int i = 0; i < str1.length(); ++i) {
            int str1CurVal = legend[(str1.read_at(i).LData & 31) - 1];
            int str2CurVal = legend[(str2.read_at(i).LData & 31) - 1];
            if(str1CurVal > str2CurVal){
                str1HighValCount++;
            }
            else if (str1CurVal < str2CurVal){
                str2HighValCount++;
            }
        }

        if(str1HighValCount > str2HighValCount){
            return false;
        }
        else if(str1HighValCount < str2HighValCount){
            return true;
        }
    }

    return false;
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
        answers.emplace_back(LString(curWord).xVals_to_subscript());
    }
    englishWords.close();

    sort(answers.begin(), answers.end(), myComp);

    int rowSubscript = 0;
    for (const auto& row : board) {
        wordsOfRow[rowSubscript].clear();
        LString rowCpy = row;
        for (auto& word: answers) {
            if(word.isDescendentOf(hand, row) && word.place_into_row(rowCpy)) {
                wordsOfRow[rowSubscript].push_back(word);
            }
            rowCpy = row;
        }
        rowSubscript++;
    }
}

string BoardReader::to_string() const {
    string buffer = "Hand: " + hand.to_string() + "\n";
    buffer += "Best Word: " + bestWord.to_string() + " - " + ::to_string(bestWord.get_points());
    buffer += "\nPostion X: " + ::to_string(bestX);
    buffer += "\nPostion Y: " + ::to_string(bestY);
    if(horizontal)
        buffer += "\nHorizontal";
    else
        buffer += "\nVertical";

    return buffer;
}

LString BoardReader::update_best_word(){
    int rowSubscript = 0;
    for (int i = 0; i < 15; ++i) {
        for (const auto& word: wordsOfRow[i]) {
            if (word.get_points() > bestWord.get_points()) {
                bestWord = word;
                bestX = bestWord[0].x + 1;
                bestY = rowSubscript + 1;
                horizontal = true;
            } else if (word.get_points() == bestWord.get_points()) {
                if (word.length() < bestWord.length() || bestWord.is_empty()) {
                    bestWord = word;
                    bestX = bestWord[0].x + 1;
                    bestY = rowSubscript + 1;
                    horizontal = true;
                }
            }
        }
        rowSubscript++;
    }

    return bestWord;
}

LString BoardReader::get_words_of_row(int subscript) {
    LString buffer;
    for (const auto& word: wordsOfRow[subscript]) {
        buffer += word + '\n';
    }

    return buffer;
}
