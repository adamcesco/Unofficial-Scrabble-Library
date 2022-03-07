//
// Created by misc1 on 3/3/2022.
//

#include "ScrabbleReader.h"

void ScrabbleReader::search_for_intersecting_words() {if(answerSet.empty())
        throw invalid_argument("Error in ScrabbleReader::search_for_intersecting_words() | The set of all scrabble words is empty.");

    int rowSubscript = 0;
    for (const auto &row: board) {
        if(row.is_blank_LStr()) {
            rowSubscript++;
            continue;
        }
        for (auto word : answerSet) {
            if (contains_letter_of_hand(word)) {
                word.set_y_vals_equal_to(rowSubscript);
                unordered_map<LString, LString, MyHashFunction> toPush = return_all_fitted_filled_rows(word,
                                                                                                       rowSubscript);
                for (auto& it: toPush) {
                    LString first = it.first;
                    LString second = it.second;

                    if(first.is_descendent_of(hand, row)) {
                        wordSets[rowSubscript].push_back(second);
                    }
                }
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
    englishWords.open("../Data/scrabble_word_list.txt");
    if(!englishWords.is_open())
        throw invalid_argument("could not open ../Data/scrabble_word_list.txt");

    string curWord;
    while(englishWords.good()){
        getline(englishWords, curWord);
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

void ScrabbleReader::search_for_tangential_words() {
    if(answerSet.empty())
        throw invalid_argument("Error in ScrabbleReader::search_for_intersecting_words() | The set of all scrabble words is empty.");

    int rowSubscript = 0;
    for (const auto &row: board) {
        for (auto word : answerSet) {
            if (contains_letter_of_hand(word) && word.is_descendent_of(hand)) {
                vector<LString> toPush = return_all_fitted_tangential_words(word, rowSubscript);
                for (auto& it: toPush) {
                    it.set_y_vals_equal_to(rowSubscript);
                    wordSets[rowSubscript].push_back(it);
                }
            }
        }
        rowSubscript++;
    }
}

void ScrabbleReader::clear_wordSets() {
    wordSets[0].clear();
    wordSets[1].clear();
    wordSets[2].clear();
    wordSets[3].clear();
    wordSets[4].clear();
    wordSets[5].clear();
    wordSets[6].clear();
    wordSets[7].clear();
    wordSets[8].clear();
    wordSets[9].clear();
    wordSets[10].clear();
    wordSets[11].clear();
    wordSets[12].clear();
    wordSets[13].clear();
    wordSets[14].clear();
}

void ScrabbleReader::search_for_all_words() {
    if(answerSet.empty())
        throw invalid_argument("Error in ScrabbleReader::search_for_intersecting_words() | The set of all scrabble words is empty.");

    int rowSubscript = 0;
    for (const auto &row: board) {
        for (auto word : answerSet) {
            if (contains_letter_of_hand(word)) {
                word.set_y_vals_equal_to(rowSubscript);
                unordered_map<LString, LString, MyHashFunction> toPush = return_all_fitted_rows(word, rowSubscript);
                for (auto& it: toPush) {
                    LString first = it.first;
                    LString second = it.second;

                    if(first.is_descendent_of(hand, row)) {
                        wordSets[rowSubscript].push_back(second);
                    }
                }
            }
        }
        rowSubscript++;
    }
}

vector<LString> ScrabbleReader::return_all_fitted_tangential_words(LString& word, int rowSubscript) {
    int mode = 0;
    if(rowSubscript == 0)
        mode = 1;
    else if(rowSubscript == 14)
        mode = -1;

    LString rowCpy = board[rowSubscript];
    vector<LString> toReturn;
    word.xVals_to_subscript();
    word.add_to_x_vals(15 - word.length());
    for (int i = 0; i < 15 - word.length(); ++i) {
        for (int j = 15 - 1; j >= 15 - word.length(); --j) {
            rowCpy[j - i] = word[(word.length() - 1) - ((15 - 1) - j)];
        }

        bool skip = false;
        for (int j = 0; j < word.length(); ++j) {
            if(board[rowSubscript][j + word[0].x] != ' ') {
                skip = true;
            }
        }
        if(skip) {
            rowCpy = board[rowSubscript];
            word.add_to_x_vals(-1);
            continue;
        }

        for (int j = 0; j < word.length(); ++j) {
            if(mode != 1 && board[rowSubscript - 1][j + word[0].x] != ' ') {
                toReturn.push_back(word);
                break;
            }
            if(mode != -1 && board[rowSubscript + 1][j + word[0].x] != ' ') {
                toReturn.push_back(word);
                break;
            }
        }


        rowCpy = board[rowSubscript];
        word.add_to_x_vals(-1);
    }

    return toReturn;
}

unordered_map<LString, LString, MyHashFunction> ScrabbleReader::return_all_fitted_rows(LString& word, int rowSubscript) {  //fix later
    int mode = 0;
    if(rowSubscript == 0)
        mode = 1;
    else if(rowSubscript == 14)
        mode = -1;

    LString rowCpy = board[rowSubscript];
    unordered_map<LString, LString, MyHashFunction> toReturn;
    word.xVals_to_subscript();
    word.add_to_x_vals(15 - word.length());
    for (int i = 0; i < 15 - word.length(); ++i) {
        for (int j = 15 - 1; j >= 15 - word.length(); --j) {
            if(rowCpy[j - i] == ' ')
                rowCpy[j - i] = word[(word.length() - 1) - ((15 - 1) - j)];
        }

        bool skip = false;
        bool allHand = true;
        for (int j = 0; j < word.length(); ++j) {
            if(board[rowSubscript][j + word[0].x] != ' ' && board[rowSubscript][j + word[0].x] != word[j]) {
                skip = true;
                break;
            }
            else if(board[rowSubscript][j + word[0].x] != ' '){
                allHand = false;
            }
        }
        if(skip || rowCpy == board[rowSubscript] || (allHand && !rowCpy.is_descendent_of(hand))) {
            rowCpy = board[rowSubscript];
            word.add_to_x_vals(-1);
            continue;
        }

        for (int j = 0; j < word.length(); ++j) {
            if(mode != 1 && board[rowSubscript - 1][j + word[0].x] != ' ') {
                toReturn.insert(pair<LString, LString>(rowCpy, word));
                break;
            }
            if(mode != -1 && board[rowSubscript + 1][j + word[0].x] != ' ') {
                toReturn.insert(pair<LString, LString>(rowCpy, word));
                break;
            }
        }


        rowCpy = board[rowSubscript];
        word.add_to_x_vals(-1);
    }

    return toReturn;
}

unordered_map<LString, LString, MyHashFunction> ScrabbleReader::return_all_fitted_filled_rows(LString & word, int rowSubscript) {
    int mode = 0;
    if(rowSubscript == 0)
        mode = 1;
    else if(rowSubscript == 14)
        mode = -1;

    LString rowCpy = board[rowSubscript];
    unordered_map<LString, LString, MyHashFunction> toReturn;
    word.xVals_to_subscript();
    word.add_to_x_vals(15 - word.length());
    for (int i = 0; i < 15 - word.length(); ++i) {
        for (int j = 15 - 1; j >= 15 - word.length(); --j) {
            rowCpy[j - i] = word[(word.length() - 1) - ((15 - 1) - j)];
        }

        bool skip = true;
        for (int j = 0; j < word.length(); ++j) {
            if(board[rowSubscript][j + word[0].x] != ' ' && board[rowSubscript][j + word[0].x] != word[j]) {
                skip = true;
                break;
            }
            else if(board[rowSubscript][j + word[0].x] != ' ') {
                skip = false;
            }
        }
        if(skip || rowCpy == board[rowSubscript]) {
            rowCpy = board[rowSubscript];
            word.add_to_x_vals(-1);
            continue;
        }

        for (int j = 0; j < word.length(); ++j) {
            if(mode != 1 && board[rowSubscript - 1][j + word[0].x] != ' ') {
                toReturn.insert(pair<LString, LString>(rowCpy, word));
                break;
            }
            if(mode != -1 && board[rowSubscript + 1][j + word[0].x] != ' ') {
                toReturn.insert(pair<LString, LString>(rowCpy, word));
                break;
            }
        }


        rowCpy = board[rowSubscript];
        word.add_to_x_vals(-1);
    }

    return toReturn;
}
