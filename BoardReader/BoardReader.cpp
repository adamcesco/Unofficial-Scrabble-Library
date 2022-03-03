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
        answerSet.emplace(LString(curWord));
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
        answerSet.emplace(LString(curWord));
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

    //build rotated board
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
    if(answerSet.empty())
        throw invalid_argument("Error in BoardReader::search_board_for_words() | The set of all scrabble words is empty.");

    if(readerType == HORIZONTAL) {
        int rowSubscript = 0;
        for (const auto &row: board) {
            wordsOfRow[rowSubscript].clear();
            for (auto word: answerSet) {
                if (word.place_into_row(row).isDescendentOf(hand, row)) {
                    word.set_y_vals_equal_to(rowSubscript);
                    wordsOfRow[rowSubscript].push_back(word);
                }
            }
            rowSubscript++;
        }
    }else if(readerType == VERTICAL){
        int rowSubscript = 0;
        for (const auto &row: board) {
            wordsOfCol[rowSubscript].clear();
            for (auto word: answerSet) {
                if (word.place_into_row(row).isDescendentOf(hand, row)) {
                    word.set_y_vals_equal_to(rowSubscript);
                    wordsOfCol[rowSubscript].push_back(word);
                }
            }
            rowSubscript++;
        }
    }
}

string BoardReader::to_string() const {
    string buffer = "Hand: " + hand.to_string() + "\n";
    buffer += "Best Vertical Word: " + bestVWord.to_string() + " - " + ::to_string(bestVWord.get_letter_points() +
                                                                                   perpendicular_points(bestVWord, VERTICAL));
    buffer += "\n\tPostion X: " + ::to_string(bestHX);
    buffer += "\n\tPostion Y: " + ::to_string(bestHY);
    buffer += "\n\tVertical";

    buffer += "\nBest Horizontal Word: " + bestHWord.to_string() + " - " + ::to_string(bestHWord.get_letter_points() +
                                                                                       perpendicular_points(bestHWord, HORIZONTAL));
    buffer += "\n\tPostion X: " + ::to_string(bestVX);
    buffer += "\n\tPostion Y: " + ::to_string(bestVY);
    buffer += "\n\tHorizontal";

    return buffer;
}

LString BoardReader::update_best_vir_word(){
    int rowSubscript = 0;
    bestVWord.clear();
    for (auto & wordSet : wordsOfCol) {
        for (const auto& word: wordSet) {
            int wordPoints = word.get_letter_points() + perpendicular_points(word, VERTICAL);
            int bestWordPoints = bestVWord.get_letter_points() + perpendicular_points(bestVWord, VERTICAL);

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

void BoardReader::check_hor_words_perpendicular() {
    for (auto & wordSet : wordsOfRow) {
        for (auto& word: wordSet) {
            vector<LString> boardCpy = return_board_with(word, HORIZONTAL);

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
                    if(shard.length() > 1 && answerSet.find(shard) == answerSet.end()){
                        word.clear();
                    }
                }
            }
        }
    }
}

void BoardReader::check_vir_words_perpendicular() {
    for (auto & wordSet : wordsOfCol) {
        for (auto& word: wordSet) {
            vector<LString> boardCpy = return_board_with(word, VERTICAL);

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
                    if(shard.length() > 1 && answerSet.find(shard) == answerSet.end()){
                        word.clear();
                    }
                }
            }
        }
    }
}

vector<LString> BoardReader::return_board_with(const LString& toPrint, Type wordType) const{    //only works if readerType == wordType
//    vector<LString> boardCpy = board;
//    int toPrintX = toPrint.read_at(0).x;
//    int toPrintY = toPrint.read_at(0).y;
//    for (int i = toPrintX; i < toPrint.length() + toPrintX; i++) {
//        if(boardCpy[toPrintY][i] == ' ')
//            boardCpy[toPrintY][i] = toPrint.read_at(i - toPrintX);
//    }
//
//    return boardCpy;
//

    vector<LString> boardCpy = board;
    if(wordType == readerType) {
        for (int i = toPrint.read_at(0).x; i < toPrint.length() + toPrint.read_at(0).x; i++) {
            if (boardCpy[toPrint.read_at(0).y][i] == ' ')
                boardCpy[toPrint.read_at(0).y][i] = Letter(toPrint.read_at(i - toPrint.read_at(0).x).LData,
                                                        i,
                                                        toPrint.read_at(0).y, toPrint.read_at(i - toPrint.read_at(0).x).flag);
        }
    }
    else if (readerType == HORIZONTAL){     //needs to be fixed
        for (int i = toPrint.read_at(0).x; i < toPrint.length() + toPrint.read_at(0).x; i++) {
            if (boardCpy[i][14 - toPrint.read_at(0).y] == ' ')
                boardCpy[i][14 - toPrint.read_at(0).y] = Letter(toPrint.read_at(i - toPrint.read_at(0).x).LData,
                                                         14 - toPrint.read_at(0).y,
                                                         i, toPrint.read_at(i - toPrint.read_at(0).x).flag);
        }
    }
    else if (readerType == VERTICAL){
        for (int i = toPrint.read_at(0).x; i < toPrint.length() + toPrint.read_at(0).x; i++) {
            if (boardCpy[14 - i][toPrint.read_at(0).y] == ' ')
                boardCpy[14 - i][toPrint.read_at(0).y] = Letter(toPrint.read_at(i - toPrint.read_at(0).x).LData,
                                                         14 - i,
                                                         toPrint.read_at(0).y, toPrint.read_at(i - toPrint.read_at(0).x).flag);
        }
    }

    return boardCpy;
}

int BoardReader::perpendicular_points(const LString &word, Type wordType) const {
    vector<LString> boardCpy = return_board_with(word, wordType);

    int sum = 0;
    for (int i = 0; i < 15; i++) {  //i = x
        LString column;
        for (int j = 0; j < 15; j++) {  //j = y
            column += boardCpy[j][i];
        }

        vector<LString> colShards = column.break_into_frags();

        for (const auto& shard : colShards) {
            if(shard.contains_flag(-1) && shard.length() > 1){
                sum += shard.get_letter_points();
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
    for (auto & words : wordsOfCol) {
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
        answerSet.emplace(LString(curWord));
    }
    englishWords.close();
}

void BoardReader::place_best_word_into_board() {
    LString bestWord;
    Type wordType;
    int HWordPoints = bestHWord.get_letter_points() + perpendicular_points(bestHWord, HORIZONTAL);
    int VWordPoints = bestVWord.get_letter_points() + perpendicular_points(bestVWord, VERTICAL);

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

    answerSet.clear();
    ifstream englishWords;
    englishWords.open("../data/englishWords.txt");
    if(!englishWords.is_open())
        throw invalid_argument("could not open ../data/englishWords.txt");

    string curWord;
    while(!englishWords.eof()){
        englishWords >> curWord;
        answerSet.emplace(LString(curWord));
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
    else if (readerType == HORIZONTAL){     //needs to be fixed
        for (int i = toPrint.read_at(0).x; i < toPrint.length() + toPrint.read_at(0).x; i++) {
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
    int rowSubscript = 0;
    bestHWord.clear();
    for (auto & wordSet : wordsOfRow) {
        for (const auto& word: wordSet) {
            int wordPoints = word.get_letter_points() + perpendicular_points(word, HORIZONTAL);
            int bestWordPoints = bestHWord.get_letter_points() + perpendicular_points(bestHWord, HORIZONTAL);

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

LString BoardReader::return_row_with(const LString &toPrint, int rowSub) const {
    int toPrintX = toPrint.read_at(0).x;
    LString temp = board[rowSub];
    for (int i = toPrintX; i < toPrint.length() + toPrintX; i++) {
        if(temp[i] == ' ')
            temp[i] = toPrint.read_at(i - toPrintX);
    }

    return temp;
}

bool BoardReader::contains_letter_of_hand(const LString &passed) const {
    unordered_set<char> handSet;
    for (int i = 0; i < hand.length(); ++i)
        handSet.emplace(toupper(hand.read_at(i).LData));

    for (int i = 0; i < passed.length(); ++i) {
        if (handSet.find(toupper(passed.read_at(i).LData)) != handSet.end()) {
            return true;
        }
    }
    return false;
}

bool BoardReader::contains_letter_of_hand(const string& passed) const {
    unordered_set<char> handSet;
    for (int i = 0; i < hand.length(); ++i)
        handSet.emplace(toupper(hand.read_at(i).LData));

    for (char i : passed) {
        if(handSet.find(toupper(i)) != handSet.end()) {
            return true;
        }
    }
    return false;
}

void BoardReader::filter_scrabble_words_by_hand() {
    auto i = answerSet.begin();
    while(i != answerSet.end()){
        auto current = i++;
        if(!contains_letter_of_hand(*current)){
            answerSet.erase(current);
        }
    }
}
