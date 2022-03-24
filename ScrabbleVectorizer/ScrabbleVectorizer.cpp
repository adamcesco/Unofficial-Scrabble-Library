#include "ScrabbleVectorizer.h"

void ScrabbleVectorizer::search_for_intersecting_words() {
    //TODO: Remove as many if-statements as possible.
    // Convert AnchoredString into AnchoredTString, so that ScrabbleDataset is composed of pre-defined TStrings.
    // Turn rackCount into a class/struct.

    if(scrabbleWordSet.empty())
        throw invalid_argument("Error in ScrabbleVectorizer::search_for_intersecting_words() | The set of all scrabble words is empty.");

    int rowSubscript = 0;
    for (auto& row: board) {
        int tileCount = 0;
        for (auto& tile : row) {
            if(tile == ' ') {
                tileCount++;
                continue;
            }

            vector<AnchoredString> wordsOfTile = wordDataset.return_this_at(tileCount, tile.letter);
            for (const auto& it : wordsOfTile) {
                TString curTStr;
                int anchorIndex = it.second;
                int rackCount[27];
                int blankCount = 0;
                for (int i = 0; i < 27; ++i) {
                    rackCount[i] = 0;
                }
                for (int i = 0; i < rack.length(); ++i) {
                    if(rack[i] == '?')
                        blankCount++;
                    else
                        rackCount[abs(rack[i]) & 31]++;
                }

                bool skip = false;
                int start = 0 - anchorIndex;
                int end = it.first.length() - anchorIndex;
                for (int i = start; i < end; ++i) {
                    char curChar = abs(it.first[i + anchorIndex]);
                    Tile curBoardTile = board[rowSubscript][tileCount + i];

                    if(curBoardTile == curChar) {
                        curTStr += Tile(curChar, tileCount + i, rowSubscript, -1);
                    }
                    else if (curBoardTile != ' '){
                        skip = true;
                        break;
                    }
                    else{
                        curTStr += Tile(curChar, tileCount + i, rowSubscript, -1);

                        if(rackCount[curChar & 31] == 0){
                            if(blankCount == 0){
                                skip = true;
                                break;
                            }
                            blankCount--;
                            curTStr.back().points = 0;
                            curTStr.back().isBlank = true;
                        }
                        else
                            rackCount[curChar & 31]--;
                    }
                }
                if(skip)
                    continue;

                moveSets[rowSubscript].push_back(curTStr);    //problem: anchored index must be 0 for the word to be passed on
            }

            tileCount++;
        }
        rowSubscript++;
    }
}

void ScrabbleVectorizer::reset_all_data() {
    bestX = bestY = 8;
    bestWord.clear();
    rack.clear();
    board.clear();

    for (auto & words : moveSets) {
        words.clear();
    }

    scrabbleWordSet.clear();
    ifstream englishWords;
    englishWords.open("../Data/scrabble_word_list.txt");
    if(!englishWords.is_open())
        throw invalid_argument("could not open ../Data/scrabble_word_list.txt");

    string curWord;
    while(englishWords.good()){
        getline(englishWords, curWord);
        scrabbleWordSet.emplace(curWord);
    }
    englishWords.close();
}

void ScrabbleVectorizer::place_into_board(const TString &toPrint) {
    for (int i = toPrint.read_at(0).x; i < toPrint.length() + toPrint.read_at(0).x; i++) {
        if (board[toPrint.read_at(0).y][i] == ' ')
            board[toPrint.read_at(0).y][i] = Tile(toPrint.read_at(i - toPrint.read_at(0).x).letter,
                                                  i,
                                                  toPrint.read_at(0).y, 1);
        perkBoard[toPrint.read_at(0).y][i] = ' ';
    }
}

bool ScrabbleVectorizer::contains_letter_of_hand(const TString &passed) const {
    unordered_set<char> handSet;
    for (int i = 0; i < rack.length(); ++i)
        handSet.emplace(toupper(rack[i]));

    for (int i = 0; i < passed.length(); ++i) {
        if (handSet.find(toupper(passed.read_at(i).letter)) != handSet.end()) {
            return true;
        }
    }
    return false;
}

vector<TString> ScrabbleVectorizer::return_raw_board_with(const TString &toPrint) const {
    vector<TString> boardCpy = board;
    int toPrintX = toPrint.read_at(0).x;
    int toPrintY = toPrint.read_at(0).y;
    for (int i = toPrintX; i < toPrint.length() + toPrintX; i++) {
        if(boardCpy[toPrintY][i] == ' ') {
            boardCpy[toPrintY][i] = toPrint.read_at(i - toPrintX);
            boardCpy[toPrintY][i].flag = -2;
        }
        else
            boardCpy[toPrintY][i].flag = 1;
    }

    return boardCpy;
}

void ScrabbleVectorizer::search_for_tangential_words() {    //does not support blank tiles
    //TODO: Remove as many if-statements as possible.
    // Convert AnchoredString into AnchoredTString, so that ScrabbleDataset is composed of pre-defined TStrings.
    // Turn rackCount into a class/struct.
    // Implement custom data-structure for tile placement checking for tangential words only.
    if(scrabbleWordSet.empty())
        throw invalid_argument("Error in ScrabbleVectorizer::search_for_tangential_words() | The set of all scrabble words is empty.");

    if(rack.length() == 1)
        return;

    RMAC rackMap(rack);

    for (int i = 0; i < 15; ++i) {
        if(i - 1 < 0)
            continue;

        for (int j = 0; j < 15; ++j) {
            for (auto &it: rackMap.data) {
                int start = it.read_at(0).x + j;
                int end = it.read_back().x + j;

                if (start < 0 || end > 14)
                    continue;

                bool skip = false;
                for (int k = 0; k < it.length(); ++k) {
                    if (board[i - 1][start + k] != ' ') {
                        skip = true;
                        break;
                    }
                    it[k].x = start + k;
                    it[k].y = i - 1;
                }
                if (skip)
                    continue;

                moveSets[i - 1].push_back(it);
            }
        }
    }
}

void ScrabbleVectorizer::clear_wordSets() {
    moveSets[0].clear();
    moveSets[1].clear();
    moveSets[2].clear();
    moveSets[3].clear();
    moveSets[4].clear();
    moveSets[5].clear();
    moveSets[6].clear();
    moveSets[7].clear();
    moveSets[8].clear();
    moveSets[9].clear();
    moveSets[10].clear();
    moveSets[11].clear();
    moveSets[12].clear();
    moveSets[13].clear();
    moveSets[14].clear();
}

int ScrabbleVectorizer::points_of_word(const TString &word) const{
    // If a letter is shared between words, then count it's premium value for all words
    // Any word multiplier only gets assigned to the original word, and not any subsequently formed words
    // If a word is placed on two or more multiplier tiles, the words value is multiplied by both tile values
    // If a word uses all the tiles in the rack then 50 is added to the final total

    if(word.is_empty())
        return 0;

    int crossWordSum = 0;
    vector<TString> boardCpy = return_raw_board_with(word);
    for (int i = 0; i < 15; ++i) {
        TString column;
        for (int j = 0; j < 15; ++j) {
            column += boardCpy[j].read_at(i);
        }

        vector<TString> colShards = column.fragments();

        for (const auto& shard : colShards) {
            if(shard.contains_flag(-2) && shard.length() > 1){
                int firstY = shard.read_at(0).y;
                for (int j = firstY; j < firstY + shard.length(); ++j) {
                    char curPerk = perkBoard[j][i];
                    if(isalpha(curPerk) && j == word.read_at(0).y)
                        crossWordSum += shard.read_at(j - firstY).points * (curPerk & 31);
                    else{
                        crossWordSum += shard.read_at(j - firstY).points;
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
            wordSum += word.read_at(i - firstX).points * (curPerk & 31);
        else if(isdigit(curPerk)) {
            multiplier *= curPerk & 15;
            wordSum += word.read_at(i - firstX).points;
        }
        else
            wordSum += word.read_at(i - firstX).points;

        if(board[firstY].read_at(i) != ' ')
            letterCount++;
    }
    wordSum *= multiplier;
    wordSum += crossWordSum;
    int handLen = rack.length();
    if(handLen == 7 && word.length() - letterCount == handLen)
        wordSum += 50;


    return wordSum;
}

vector<int> ScrabbleVectorizer::find_points_of_word (const string& passed) const {
    vector<int> values;
    for (auto & curWordSet : moveSets) {
        for (const auto& word : curWordSet) {
            if(word == passed){
                values.push_back(points_of_word(word));
            }
        }
    }
    return values;
}

void ScrabbleVectorizer::place_best_word_into_board() {
    for (int i = bestWord.read_at(0).x; i < bestWord.length() + bestWord.read_at(0).x; i++) {
        if (board[bestWord.read_at(0).y][i] == ' ')
            board[bestWord.read_at(0).y][i] = Tile(bestWord.read_at(i - bestWord.read_at(0).x).letter,
                                                   i,
                                                   bestWord.read_at(0).y, 1);
        perkBoard[bestWord.read_at(0).y][i] = ' ';
    }
}

vector<string> ScrabbleVectorizer::return_raw_perkBoard_copy() {
    vector<string> toReturn;
    for (int i = 0; i < 15; ++i) {
        string row;
        for (int j = 0; j < 15; ++j) {
            row += perkBoard[i][j];
        }
        toReturn.push_back(row);
    }
    return toReturn;
}

vector<string> ScrabbleVectorizer::return_raw_char_board_copy() {
    vector<string> toReturn;
    for (int i = 0; i < 15; ++i) {
        string row;
        for (int j = 0; j < 15; ++j) {
            row += board[i][j].letter;
        }
        toReturn.push_back(row);
    }
    return toReturn;
}

vector<TString> ScrabbleVectorizer::return_all_of_raw_word(const string& passed) const {
    vector<TString> foundVersions;
    for (auto & curWordSet : moveSets) {
        for (const auto& word : curWordSet) {
            if(word == passed){
                foundVersions.push_back(word);
            }
        }
    }
    return foundVersions;
}
