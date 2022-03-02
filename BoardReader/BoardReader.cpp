//
// Created by misc1 on 2/26/2022.
//

#include "BoardReader.h"
#include <sstream>

BoardReader::BoardReader() {
    bestHX = bestHY = 8;
    readerType = UNDEFINED;

    ifstream englishWords;
    englishWords.open("../data/englishWords.txt");
    if(!englishWords.is_open())
        throw invalid_argument("could not open ../data/englishWords.txt");

    string curWord;
    while(!englishWords.eof()){
        englishWords >> curWord;
        answerSet.emplace(curWord);
    }
    englishWords.close();
}

BoardReader::BoardReader(const LString& passed) {
    hand = passed;
    bestHX = bestHY = 8;
    readerType = UNDEFINED;

    ifstream englishWords;
    englishWords.open("../data/englishWords.txt");
    if(!englishWords.is_open())
        throw invalid_argument("could not open ../data/englishWords.txt");

    string curWord;
    while(!englishWords.eof()){
        englishWords >> curWord;
        answerSet.emplace(curWord);
    }
    englishWords.close();
}

void BoardReader::build_board() {
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
    readerType = HORIZONTAL;
}

void BoardReader::print_board() const{
    if(readerType == HORIZONTAL) {
        for (const auto &row: board) {
            for (int i = 0; i < row.length(); ++i) {
                cout << row.read_at(i).LData;
            }
            cout << endl;
        }
        return;
    }else if (readerType == VERTICAL){
        for (int i = 0; i < 15; ++i) {
            for (int j = 14; j >= 0; --j) {
                cout << board[j].read_at(i).LData;
            }
            cout << endl;
        }
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

void BoardReader::search_board_for_words() {
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
        for (auto& word: answers) {
            if(word.isDescendentOf(hand, row) && word.place_into_row(row)) {
                word.set_y_vals_equal_to(rowSubscript);
                wordsOfRow[rowSubscript].push_back(word);
            }
        }
        rowSubscript++;
    }
}

string BoardReader::to_string() const {
    string buffer = "Hand: " + hand.to_string() + "\n";
    buffer += "Best Vertical Word: " + bestVWord.to_string() + " - " + ::to_string(bestVWord.get_horizontal_points() +
                                                                          perpendicular_points(bestVWord));
    buffer += "\n\tPostion X: " + ::to_string(bestHX);
    buffer += "\n\tPostion Y: " + ::to_string(bestHY);
    buffer += "\n\tVertical";

    buffer += "\nBest Horizontal Word: " + bestHWord.to_string() + " - " + ::to_string(bestHWord.get_horizontal_points() +
                                                                                       perpendicular_points(bestHWord));
    buffer += "\n\tPostion X: " + ::to_string(bestVX);
    buffer += "\n\tPostion Y: " + ::to_string(bestVY);
    buffer += "\n\tHorizontal";

    return buffer;
}

LString BoardReader::update_best_vir_word(){
    if(readerType == HORIZONTAL)
        throw invalid_argument("Called BoardReader::update_best_vir_word() when reader is currently in HORIZONTAL reading mode");
    
    int rowSubscript = 0;
    bestVWord.clear();
    for (auto & wordSet : wordsOfRow) {
        for (const auto& word: wordSet) {
            int wordPoints = word.get_horizontal_points() + perpendicular_points(word);
            int bestWordPoints = bestVWord.get_horizontal_points() + perpendicular_points(bestVWord);

            if (wordPoints > bestWordPoints) {
                bestVWord = word;
                //x = y, y = 14 - x
                bestVX = (14 - rowSubscript) + 1;
                bestVY = (bestVWord[0].x) + 1;
            } else if (wordPoints == bestWordPoints) {
                if (word.length() < bestVWord.length() || bestVWord.is_empty()) {
                    bestVWord = word;
                    //x = y, y = 14 - x
                    bestVX = (14 - rowSubscript) + 1;
                    bestVY = (bestVWord[0].x) + 1;
                }
            }
        }
        rowSubscript++;
    }

    return bestVWord;
}

LString BoardReader::get_words_of_row(int subscript) {
    LString buffer;
    for (const auto& word: wordsOfRow[subscript]) {
        if(word.is_empty()){
            continue;
        }
        buffer += word + '\n';
    }
    buffer.pop_back();

    return buffer;
}

LString BoardReader::row_to_string(int subscript) {
    LString buffer;
    for (const auto& word: wordsOfRow[subscript]) {
        if(word.is_empty()){
            continue;
        }
        buffer += word;
        buffer += " - " + ::to_string(word.get_horizontal_points() + perpendicular_points(word)) + '\n';
    }
    buffer.pop_back();

    return buffer;
}

void BoardReader::check_perpendicular_compatibility() {
    for (auto & wordSet : wordsOfRow) {
        for (auto& word: wordSet) {
            vector<LString> boardCpy = return_board_with(word);

            for (int i = 0; i < 15; i++) {  //i = x
                LString column;
                if(readerType == HORIZONTAL) {
                    for (int j = 0; j < 15; j++) {  //j = y
                        column += boardCpy[j][i];
                    }
                }
                else if (readerType == VERTICAL){
                    for (int j = 14; j >= 0; j--) {  //j = y
                        column += boardCpy[j][i];
                    }
                }

                vector<LString> colShards = column.break_into_frags();

                for (const auto& shard : colShards) {
                    if(shard.length() > 1 && answerSet.find(shard.to_string()) == answerSet.end()){
                        word.clear();
                    }
                }
            }
        }
    }
}

vector<LString> BoardReader::return_board_with(const LString& toPrint) const{
    vector<LString> boardCpy = board;
    for (int i = toPrint.read_at(0).x; i < toPrint.length() + toPrint.read_at(0).x; i++) {
        if(boardCpy[toPrint.read_at(0).y][i] == ' ')
            boardCpy[toPrint.read_at(0).y][i] = toPrint.read_at(i - toPrint.read_at(0).x);
    }

    return boardCpy;
}

int BoardReader::perpendicular_points(const LString &word) const {
    vector<LString> boardCpy = return_board_with(word);

    int sum = 0;
    for (int i = 0; i < 15; i++) {  //i = x
        LString column;
        for (int j = 0; j < 15; j++) {  //j = y
            column += boardCpy[j][i];
        }

        vector<LString> colShards = column.break_into_frags();

        for (const auto& shard : colShards) {
            if(shard.contains_flag(-1) && shard.length() > 1){
                sum += shard.get_horizontal_points();
            }
        }
    }

    return sum;
}

void BoardReader::to_vertical_reader() {
    if(readerType == VERTICAL)
        return;

    vector<LString> boardCpy;
    for (int i = 14; i >= 0; i--) {  //i = x
        LString column;
        for (int j = 0; j < 15; j++) {  //j = y
            column += Letter(board[j][i].LData, j, 14 - i, 1);
        }
        boardCpy.push_back(column);
    }
    board = boardCpy;
    readerType = VERTICAL;
}

void BoardReader::to_horizontal_reader() {
    if(readerType == HORIZONTAL)
        return;

    vector<LString> boardCpy;
    for (int i = 0; i < 15; ++i) {
        LString temp;
        for (int j = 14; j >= 0; --j) {
            temp += Letter(board[j].read_at(i).LData, 14 - j, i,1);
        }
        boardCpy.push_back(temp);
    }
    board = boardCpy;
    readerType = HORIZONTAL;
}

void BoardReader::reset_all_data() {
    bestHX = bestHY = 8;
    bestVWord.clear();
    bestHWord.clear();
    hand.clear();
    readerType = HORIZONTAL;
    board.clear();

    for (auto & words : wordsOfRow) {
        words.clear();
    }

    answerSet.clear();
    ifstream englishWords;
    englishWords.open("../data/englishWords.txt");
    if(!englishWords.is_open())
        throw invalid_argument("could not open ../data/englishWords.txt");

    string curWord;
    while(!englishWords.eof()){
        englishWords >> curWord;
        answerSet.emplace(curWord);
    }
    englishWords.close();
}

void BoardReader::place_best_word_into_board() {
    LString bestWord;
    Type wordType;
    int HWordPoints = bestHWord.get_horizontal_points() + perpendicular_points(bestHWord);
    int VWordPoints = bestVWord.get_horizontal_points() + perpendicular_points(bestVWord);

    if (HWordPoints > VWordPoints) {
        bestWord = bestHWord;
        wordType = HORIZONTAL;
    }
    else if(HWordPoints < VWordPoints) {
        bestWord = bestVWord;
        wordType = VERTICAL;
    }else {
        if (bestHWord.length() < bestVWord.length()) {
            bestWord = bestHWord;
            wordType = HORIZONTAL;
        }
        else{
            bestWord = bestVWord;
            wordType = VERTICAL;
        }
    }

    place_into_board(bestWord, wordType);
}

void BoardReader::prime_for_different_mode() {
    if(readerType == VERTICAL){
        vector<LString> boardCpy;
        for (int i = 0; i < 15; ++i) {
            LString temp;
            for (int j = 14; j >= 0; --j) {
                temp += Letter(board[j].read_at(i).LData, 14 - j, i,1);
            }
            boardCpy.push_back(temp);
        }
        board = boardCpy;
    }
    readerType = HORIZONTAL;

    for (auto & words : wordsOfRow) {
        words.clear();
    }

    answerSet.clear();
    ifstream englishWords;
    englishWords.open("../data/englishWords.txt");
    if(!englishWords.is_open())
        throw invalid_argument("could not open ../data/englishWords.txt");

    string curWord;
    while(!englishWords.eof()){
        englishWords >> curWord;
        answerSet.emplace(curWord);
    }
    englishWords.close();
}

void BoardReader::place_into_board(const LString &toPrint, Type wordType) {
    if(wordType == readerType) {
        for (int i = toPrint.read_at(0).x; i < toPrint.length() + toPrint.read_at(0).x; i++) {
            if (board[toPrint.read_at(0).y][i] == ' ')
                board[toPrint.read_at(0).y][i] = Letter(toPrint.read_at(i - toPrint.read_at(0).x).LData,
                                                        i,
                                                        toPrint.read_at(0).y, 1);
        }
    }
    else if (readerType == HORIZONTAL){
        for (int i = toPrint.read_at(0).x; i > toPrint.read_at(0).x - toPrint.length(); i--) {
            board[i][14 - toPrint.read_at(0).y] = Letter(toPrint.read_at(i - toPrint.read_at(0).x).LData,
                                                         14 - toPrint.read_at(0).y,
                                                         i, 1);
        }
    }
    else if (readerType == VERTICAL){
        for (int i = toPrint.read_at(0).x; i < toPrint.length() + toPrint.read_at(0).x; i++) {
            board[14 - i][toPrint.read_at(0).y] = Letter(toPrint.read_at(i - toPrint.read_at(0).x).LData,
                                                         14 - i,
                                                         toPrint.read_at(0).y, 1);
        }
    }
}

LString BoardReader::update_best_hor_word() {
    if(readerType == VERTICAL)
        throw invalid_argument("Called BoardReader::update_best_hor_word() when reader is currently in VERTICAL reading mode");
    
    int rowSubscript = 0;
    bestHWord.clear();
    for (auto & wordSet : wordsOfRow) {
        for (const auto& word: wordSet) {
            int wordPoints = word.get_horizontal_points() + perpendicular_points(word);
            int bestWordPoints = bestHWord.get_horizontal_points() + perpendicular_points(bestHWord);

            if (wordPoints > bestWordPoints) {
                bestHWord = word;
                bestHX = bestHWord[0].x + 1;
                bestHY = rowSubscript + 1;
            } else if (wordPoints == bestWordPoints) {
                if (word.length() < bestHWord.length() || bestHWord.is_empty()) {
                    bestHWord = word;
                    bestHX = bestHWord[0].x + 1;
                    bestHY = rowSubscript + 1;
                }
            }
        }
        rowSubscript++;
    }

    return bestHWord;
}
