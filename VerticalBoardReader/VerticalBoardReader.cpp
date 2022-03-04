//
// Created by misc1 on 3/3/2022.
//

#include "VerticalBoardReader.h"
//
// Created by misc1 on 2/26/2022.
//
#include <sstream>

VerticalBoardReader::VerticalBoardReader() {
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

VerticalBoardReader::VerticalBoardReader(const LString& passed) {
    hand = passed;

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

void VerticalBoardReader::build_board() {
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

void VerticalBoardReader::print_board() const{
    for (int i = 0; i < 15; ++i) {
        for (int j = 14; j >= 0; --j) {
            cout << board[j].read_at(i).LData;
        }
        cout << endl;
    }
}

void VerticalBoardReader::search_board_for_words() {
    if(answerSet.empty())
        throw invalid_argument("Error in VerticalBoardReader::search_board_for_words() | The set of all scrabble words is empty.");

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

string VerticalBoardReader::to_string() const {
    string buffer = "Hand: " + hand.to_string() + "\n";
    buffer += "Best Vertical Word: " + bestVWord.to_string() + " - " + ::to_string(bestVWord.get_letter_points() +
                                                                                   perpendicular_points(bestVWord));
    buffer += "\n\tPostion X: " + ::to_string(bestVX);
    buffer += "\n\tPostion Y: " + ::to_string(bestVY);
    buffer += "\n\tVertical";

    return buffer;
}

LString VerticalBoardReader::update_best_vir_word(){
    int rowSubscript = 0;
    bestVWord.clear();
    for (auto & wordSet : wordsOfCol) {
        for (const auto& word: wordSet) {
            int wordPoints = word.get_letter_points() + perpendicular_points(word);
            int bestWordPoints = bestVWord.get_letter_points() + perpendicular_points(bestVWord);

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

void VerticalBoardReader::check_vir_words_perpendicular() {
    for (auto & wordSet : wordsOfCol) {
        for (auto& word: wordSet) {
            vector<LString> boardCpy = return_board_with(word);

            for (int i = 0; i < 15; i++) {  //i = x
                LString column;
                for (int j = 14; j >= 0; j--) {  //j = y
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

vector<LString> VerticalBoardReader::return_board_with(const LString& toPrint) const{    //only works if readerType == wordType
    vector<LString> boardCpy = board;
    int toPrintX = toPrint.read_at(0).x;
    int toPrintY = toPrint.read_at(0).y;
    for (int i = toPrintX; i < toPrint.length() + toPrintX; i++) {
        if(boardCpy[toPrintY][i] == ' ')
            boardCpy[toPrintY][i] = toPrint.read_at(i - toPrintX);
    }

    return boardCpy;
}

int VerticalBoardReader::perpendicular_points(const LString &word) const {
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

void VerticalBoardReader::reset_all_data() {
    bestVWord.clear();
    hand.clear();
    board.clear();

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

void VerticalBoardReader::place_into_board(const LString &toPrint) {
    for (int i = toPrint.read_at(0).x; i < toPrint.length() + toPrint.read_at(0).x; i++) {
        if (board[toPrint.read_at(0).y][i] == ' ')
            board[toPrint.read_at(0).y][i] = Letter(toPrint.read_at(i - toPrint.read_at(0).x).LData,
                                                    i,
                                                    toPrint.read_at(0).y, 1);
    }
}

LString VerticalBoardReader::return_row_with(const LString &toPrint, int rowSub) const {
    int toPrintX = toPrint.read_at(0).x;
    LString temp = board[rowSub];
    for (int i = toPrintX; i < toPrint.length() + toPrintX; i++) {
        if(temp[i] == ' ')
            temp[i] = toPrint.read_at(i - toPrintX);
    }

    return temp;
}

bool VerticalBoardReader::contains_letter_of_hand(const LString &passed) const {
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

bool VerticalBoardReader::contains_letter_of_hand(const string& passed) const {
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

void VerticalBoardReader::filter_scrabble_words_by_hand() {
    auto i = answerSet.begin();
    while(i != answerSet.end()){
        auto current = i++;
        if(!contains_letter_of_hand(*current)){
            answerSet.erase(current);
        }
    }
}
