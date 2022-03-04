//
// Created by misc1 on 3/3/2022.
//

#include "ScrabbleReader.h"

ScrabbleReader::ScrabbleReader() {
    bestX = bestY = -1;
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

ScrabbleReader::ScrabbleReader(const LString &passed) {
    hand = passed;
    bestX = bestY = -1;

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

void ScrabbleReader::search_board_for_words() { //calls too many functions
    if(answerSet.empty())
        throw invalid_argument("Error in ScrabbleReader::search_board_for_words() | The set of all scrabble words is empty.");

    int rowSubscript = 0;
    for (const auto &row: board) {
        wordSets[rowSubscript].clear();
        for (auto word: answerSet) {
            if (word.place_into_row(row).isDescendentOf(hand, row)) {
                word.set_y_vals_equal_to(rowSubscript);
                wordSets[rowSubscript].push_back(word);
            }
        }
        rowSubscript++;
    }
}

void ScrabbleReader::reset_all_data() {
    bestX = bestY = 8;
    bestWord.clear();
    hand.clear();
    board.clear();

    for (auto & words : wordSets) {
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

void ScrabbleReader::place_into_board(const LString &toPrint) {
    for (int i = toPrint.read_at(0).x; i < toPrint.length() + toPrint.read_at(0).x; i++) {
        if (board[toPrint.read_at(0).y][i] == ' ')
            board[toPrint.read_at(0).y][i] = Letter(toPrint.read_at(i - toPrint.read_at(0).x).LData,
                                                    i,
                                                    toPrint.read_at(0).y, 1);
    }
}

void ScrabbleReader::filter_scrabble_words_by_hand() {
    auto i = answerSet.begin();
    while(i != answerSet.end()){
        auto current = i++;
        if(!contains_letter_of_hand(*current)){
            answerSet.erase(current);
        }
    }
}

LString ScrabbleReader::return_row_with(const LString &toPrint, int rowSub) const {
    int toPrintX = toPrint.read_at(0).x;
    LString temp = board[rowSub];
    for (int i = toPrintX; i < toPrint.length() + toPrintX; i++) {
        if(temp[i] == ' ')
            temp[i] = toPrint.read_at(i - toPrintX);
    }

    return temp;
}

bool ScrabbleReader::contains_letter_of_hand(const LString &passed) const {
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

bool ScrabbleReader::contains_letter_of_hand(const string &passed) const {
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

vector<LString> ScrabbleReader::return_board_with(const LString &toPrint) const {
    vector<LString> boardCpy = board;
    int toPrintX = toPrint.read_at(0).x;
    int toPrintY = toPrint.read_at(0).y;
    for (int i = toPrintX; i < toPrint.length() + toPrintX; i++) {
        if(boardCpy[toPrintY][i] == ' ')
            boardCpy[toPrintY][i] = toPrint.read_at(i - toPrintX);
    }

    return boardCpy;
}

int ScrabbleReader::perpendicular_points(const LString &word) const {
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
