//
// Created by misc1 on 2/26/2022.
//

#include "HorizontalBoardReader.h"
#include <sstream>

HorizontalBoardReader::HorizontalBoardReader() {
    bestX = bestY = 8;

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

HorizontalBoardReader::HorizontalBoardReader(const LString& passed) {
    hand = passed;
    bestX = bestY = 8;

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

void HorizontalBoardReader::build_board() {
    ifstream boardFile;
    boardFile.open("../Data/Board.csv");
    if(!boardFile.is_open())
        throw invalid_argument("could not open ../Data/Board.csv");

    string row;
    int rowCount = 0;
    while (boardFile.good()){
        getline(boardFile, row);
        string cell;
        stringstream strStr(row);
        int cellCount = 0;
        LString rowVect;
        while (getline(strStr, cell, ',')){
            if(!cell.empty() && isalpha(cell[0])) {
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

    for (int i = 0; i < 15; ++i) {
        for (int j = 0; j < 15; ++j) {
            if(board[i][j] != ' ')
                perkBoard[i][j] = ' ';
        }
    }
}

void HorizontalBoardReader::print_formatted_board() const{
    for (const auto &row: board) {
        for (int i = 0; i < row.length(); ++i) {
            cout << row.read_at(i).LData;
        }
        cout << endl;
    }
}

string HorizontalBoardReader::to_string() const {
    string buffer = "Hand: " + hand.to_string() + "\n";
    buffer += "\nBest Horizontal Word: " + bestWord.to_string() + " - " + ::to_string(bestWord.get_letter_points() +
                                                                                      perpendicular_points(bestWord));
    buffer += "\n\tPostion X: " + ::to_string(bestX);
    buffer += "\n\tPostion Y: " + ::to_string(bestY);
    buffer += "\n\tHorizontal";

    return buffer;
}

void HorizontalBoardReader::validate_words() {
    for (auto & wordSet : wordSets) {
        for (auto& word: wordSet) {
            vector<LString> boardCpy = return_raw_board_with(word);

            for (int i = 0; i < 15; i++) {
                LString row;
                LString column;
                for (int j = 0; j < 15; ++j) {
                    row += boardCpy[i].read_at(j);
                    column += boardCpy[j].read_at(i);
                }

                vector<LString> colShards = column.break_into_frags();

                for (const auto& shard : colShards) {
                    if(shard.length() > 1 && answerSet.find(shard) == answerSet.end()) {
                        word.clear();
                    }
                }

                vector<LString> rowShards = row.break_into_frags();

                for (const auto& shard : rowShards) {
                    if(shard.length() > 1 && answerSet.find(shard) == answerSet.end()) {
                        word.clear();
                    }
                }
            }
        }
    }
}

LString HorizontalBoardReader::update_best_word() {
    int rowSubscript = 0;
    bestWord.clear();
    for (auto & wordSet : wordSets) {
        for (const auto& word: wordSet) {
            int wordPoints = points_of_word(word);
            int bestWordPoints = points_of_word(bestWord);

            if (wordPoints > bestWordPoints) {
                bestWord = word;
                bestX = bestWord[0].x + 1;
                bestY = rowSubscript + 1;
            } else if (wordPoints == bestWordPoints) {
                if (word.length() < bestWord.length() || bestWord.is_empty()) {
                    bestWord = word;
                    bestX = bestWord[0].x + 1;
                    bestY = rowSubscript + 1;
                }
            }
        }
        rowSubscript++;
    }

    return bestWord;
}

void HorizontalBoardReader::validate_board() const{
    if(answerSet.empty())
        throw invalid_argument("Error in ScrabbleReader::validate_board() | unordered_map<LString> answerSet is empty.");

    for (int i = 0; i < 15; ++i) {
        LString row;
        for (int j = 0; j < 15; ++j) {
            row += board[i].read_at(j);
        }

        LString column;
        for (int j = 0; j < 15; ++j) {
            column += board[j].read_at(i);
        }

        vector<LString> colShards = column.break_into_frags();

        for (const auto& shard : colShards) {
            if(shard.length() > 1 && answerSet.find(shard) == answerSet.end())
                throw invalid_argument("Invalid vertical Word in Data/Board.csv | " + shard.to_string());
        }

        vector<LString> rowShards = row.break_into_frags();

        for (const auto& shard : rowShards) {
            if(shard.length() > 1 && answerSet.find(shard) == answerSet.end())
                throw invalid_argument("Invalid horizontal Word in Data/Board.csv | " + shard.to_string());
        }
    }
}

int HorizontalBoardReader::points_of_word(const LString &word) {
    // If a letter is shared between words, then count it's premium value for all words
    // Any word multiplier only gets assigned to the original word, and not any subsequently formed words
    // If a word is placed on two or more multiplier tiles, the words value is multiplied by both tile values
    // If a word uses all the tiles in the hand then 50 is added to the final total

    if(word.is_empty())
        return 0;

    int crossWordSum = 0;
    vector<LString> boardCpy = return_raw_board_with(word);
    for (int i = 0; i < 15; ++i) {
        LString column;
        for (int j = 0; j < 15; ++j) {
            column += boardCpy[j].read_at(i);
        }

        vector<LString> colShards = column.break_into_frags();

        for (const auto& shard : colShards) {
            if(shard.contains_flag(-1) && shard.length() > 1){
                int firstY = shard.read_at(0).y;
                for (int j = firstY; j < firstY + shard.length(); ++j) {
                    char curPerk = perkBoard[j][i];
                    if(isalpha(curPerk) && j == word.read_at(0).y)
                        crossWordSum += shard.read_at(j - firstY).val * (curPerk & 31);
                    else{
                        crossWordSum += shard.read_at(j - firstY).val;
                    }
                }
            }
        }
    }

    int wordSum = 0;
    int multiplier = 1;
    char letterCount = 0;
    int firstX = word.read_at(0).x;
    int firstY = word.read_at(0).y;
    for (int i = firstX; i < firstX + word.length(); ++i) {
        char curPerk = perkBoard[firstY][i];
        if(isalpha(curPerk))
            wordSum += word.read_at(i - firstX).val * (curPerk & 31);
        else if(isdigit(curPerk)) {
            multiplier *= curPerk & 15;
            wordSum += word.read_at(i - firstX).val;
        }
        else
            wordSum += word.read_at(i - firstX).val;

        if(board[firstY][i] != ' ')
            letterCount++;
    }
    wordSum *= multiplier;
    wordSum += crossWordSum;
    if(word.length() - letterCount == 7)
        wordSum += 50;


    return wordSum;
}
