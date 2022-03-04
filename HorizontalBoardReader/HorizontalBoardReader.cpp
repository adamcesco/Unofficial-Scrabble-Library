//
// Created by misc1 on 2/26/2022.
//

#include "HorizontalBoardReader.h"
#include <sstream>

HorizontalBoardReader::HorizontalBoardReader() {
    bestHX = bestHY = 8;

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

HorizontalBoardReader::HorizontalBoardReader(const LString& passed) {
    hand = passed;
    bestHX = bestHY = 8;

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

void HorizontalBoardReader::build_board() {
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

    //build rotated board
}

void HorizontalBoardReader::print_board() const{
    for (const auto &row: board) {
        for (int i = 0; i < row.length(); ++i) {
            cout << row.read_at(i).LData;
        }
        cout << endl;
    }
    return;
}

void HorizontalBoardReader::search_board_for_words() {
    if(answerSet.empty())
        throw invalid_argument("Error in HorizontalBoardReader::search_board_for_words() | The set of all scrabble words is empty.");

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
}

string HorizontalBoardReader::to_string() const {
    string buffer = "Hand: " + hand.to_string() + "\n";
    buffer += "\nBest Horizontal Word: " + bestHWord.to_string() + " - " + ::to_string(bestHWord.get_letter_points() +
                                                                                       perpendicular_points(bestHWord));
    buffer += "\n\tPostion X: " + ::to_string(bestHX);
    buffer += "\n\tPostion Y: " + ::to_string(bestHY);
    buffer += "\n\tHorizontal";

    return buffer;
}

void HorizontalBoardReader::check_hor_words_perpendicular() {
    for (auto & wordSet : wordsOfRow) {
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

vector<LString> HorizontalBoardReader::return_board_with(const LString& toPrint) const{    //only works if readerType == wordType
    vector<LString> boardCpy = board;
    int toPrintX = toPrint.read_at(0).x;
    int toPrintY = toPrint.read_at(0).y;
    for (int i = toPrintX; i < toPrint.length() + toPrintX; i++) {
        if(boardCpy[toPrintY][i] == ' ')
            boardCpy[toPrintY][i] = toPrint.read_at(i - toPrintX);
    }

    return boardCpy;
}

int HorizontalBoardReader::perpendicular_points(const LString &word) const {
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
                sum += shard.get_letter_points();
            }
        }
    }

    return sum;
}

void HorizontalBoardReader::reset_all_data() {
    bestHX = bestHY = 8;
    bestHWord.clear();
    hand.clear();
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
        answerSet.emplace(LString(curWord));
    }
    englishWords.close();
}

void HorizontalBoardReader::place_into_board(const LString &toPrint) {
    for (int i = toPrint.read_at(0).x; i < toPrint.length() + toPrint.read_at(0).x; i++) {
        if (board[toPrint.read_at(0).y][i] == ' ')
            board[toPrint.read_at(0).y][i] = Letter(toPrint.read_at(i - toPrint.read_at(0).x).LData,
                                                    i,
                                                    toPrint.read_at(0).y, 1);
    }
}

LString HorizontalBoardReader::update_best_hor_word() {
    int rowSubscript = 0;
    bestHWord.clear();
    for (auto & wordSet : wordsOfRow) {
        for (const auto& word: wordSet) {
            int wordPoints = word.get_letter_points() + perpendicular_points(word);
            int bestWordPoints = bestHWord.get_letter_points() + perpendicular_points(bestHWord);

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

LString HorizontalBoardReader::return_row_with(const LString &toPrint, int rowSub) const {
    int toPrintX = toPrint.read_at(0).x;
    LString temp = board[rowSub];
    for (int i = toPrintX; i < toPrint.length() + toPrintX; i++) {
        if(temp[i] == ' ')
            temp[i] = toPrint.read_at(i - toPrintX);
    }

    return temp;
}

bool HorizontalBoardReader::contains_letter_of_hand(const LString &passed) const {
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

bool HorizontalBoardReader::contains_letter_of_hand(const string& passed) const {
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

void HorizontalBoardReader::filter_scrabble_words_by_hand() {
    auto i = answerSet.begin();
    while(i != answerSet.end()){
        auto current = i++;
        if(!contains_letter_of_hand(*current)){
            answerSet.erase(current);
        }
    }
}
