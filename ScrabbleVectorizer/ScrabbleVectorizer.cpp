#include "ScrabbleVectorizer.h"

void ScrabbleVectorizer::search_for_intersecting_words() {
    if(scrabbleWordSet.empty())
        throw invalid_argument("Error in ScrabbleVectorizer::search_for_intersecting_words() | The set of all scrabble words is empty.");

    int rowSubscript = 0;
    for (auto& row: board) {
        row.set_x_vals_to_subscripts();
        TString rowCpy = row;
        rowCpy.set_x_vals_to_subscripts();

        int tileCount = 0;
        for (auto& tile : row) {
            if(tile == ' ') {
                tileCount++;
                continue;
            }

            vector<AnchoredString> wordsOfTile = wordDataset.return_this_at(rowSubscript, tileCount, tile.letter);
            for (const auto& it : wordsOfTile) {
                TString curLStr(it.first);
                int anchorIndex = it.second;

                bool skip = false;
                for (int i = 0 - anchorIndex; i < curLStr.length() - anchorIndex; ++i) {
                    if(tileCount + i > 14 || (board[rowSubscript][tileCount + i] != ' ' && board[rowSubscript][tileCount + i] != curLStr[i + anchorIndex])){
                        skip = true;
                        break;
                    }

                    curLStr[i + anchorIndex].x = tileCount + i;
                    rowCpy[tileCount + i] = curLStr[i + anchorIndex].letter;
                    rowCpy[tileCount + i].flag = -2;
                }
                if(skip)
                    continue;

                if(rowCpy.row_is_descendent_of(rack, row, curLStr)) {   //TODO: Remove the need to check if its a descendent
                    curLStr.set_y_vals_equal_to(rowSubscript);
                    answerSets[rowSubscript].push_back(curLStr);
                }
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

    for (auto & words : answerSets) {
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

void ScrabbleVectorizer::search_for_tangential_words() {
    if(scrabbleWordSet.empty())
        throw invalid_argument("Error in ScrabbleVectorizer::search_for_tangential_words() | The set of all scrabble words is empty.");

    int rowSubscript = 0;
    for (auto& row: board) {
        row.set_x_vals_to_subscripts();
        TString rowCpy = row;
        rowCpy.set_x_vals_to_subscripts();

        //computing tangential words above
        int tileCount = 0;
        for (auto& tile : row) {
            if(rowSubscript - 1 < 0 || !(tile != ' ' && board[rowSubscript - 1][tileCount] == ' ')) {
                tileCount++;
                continue;
            }

            for(int j = 0; j < rack.length(); ++j){
                vector<AnchoredString> wordsOfTile = wordDataset.return_this_at(rowSubscript, tileCount, rack[j]);

                for (auto &it: wordsOfTile) {
                    TString curLStr(it.first);
                    int anchorIndex = it.second;
                    if ((tileCount - anchorIndex) < 0 || !curLStr.is_descendent_of(rack))
                        continue;

                    bool skip = false;
                    for (int k = 0 - anchorIndex; k < curLStr.length() - anchorIndex; ++k) {
                        if (tileCount + k > 14 || board[rowSubscript - 1][tileCount + k] != ' ') {
                            skip = true;
                            break;
                        }

                        curLStr[k + anchorIndex].x = tileCount + k;
                        rowCpy[tileCount + k] = curLStr[k + anchorIndex].letter;
                        rowCpy[tileCount + k].flag = -2;
                    }
                    if (skip)
                        continue;

                    curLStr.set_y_vals_equal_to(rowSubscript - 1);
                    answerSets[rowSubscript - 1].push_back(curLStr);
                }
            }

            tileCount++;
        }
        
        //computing tangential words below
        tileCount = 0;
        for (auto& tile : row) {
            if(rowSubscript + 1 > 14 || !(tile != ' ' && board[rowSubscript + 1][tileCount] == ' ')) {
                tileCount++;
                continue;
            }

            for(int j = 0; j < rack.length(); ++j){
                vector<AnchoredString> wordsOfTile = wordDataset.return_this_at(rowSubscript, tileCount, rack[j]);

                for (auto &it: wordsOfTile) {
                    TString curLStr(it.first);
                    int anchorIndex = it.second;
                    if ((tileCount - anchorIndex) < 0 || !curLStr.is_descendent_of(rack))
                        continue;

                    bool skip = false;
                    for (int k = 0 - anchorIndex; k < curLStr.length() - anchorIndex; ++k) {
                        if (tileCount + k > 14 || board[rowSubscript + 1][tileCount + k] != ' ') {
                            skip = true;
                            break;
                        }

                        curLStr[k + anchorIndex].x = tileCount + k;
                        rowCpy[tileCount + k] = curLStr[k + anchorIndex].letter;
                        rowCpy[tileCount + k].flag = -2;
                    }
                    if (skip)
                        continue;

                    curLStr.set_y_vals_equal_to(rowSubscript + 1);
                    answerSets[rowSubscript + 1].push_back(curLStr);
                }
            }

            tileCount++;
        }
        
        rowSubscript++;
    }
}

void ScrabbleVectorizer::clear_wordSets() {
    answerSets[0].clear();
    answerSets[1].clear();
    answerSets[2].clear();
    answerSets[3].clear();
    answerSets[4].clear();
    answerSets[5].clear();
    answerSets[6].clear();
    answerSets[7].clear();
    answerSets[8].clear();
    answerSets[9].clear();
    answerSets[10].clear();
    answerSets[11].clear();
    answerSets[12].clear();
    answerSets[13].clear();
    answerSets[14].clear();
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
    for (auto & curWordSet : answerSets) {
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
    for (auto & curWordSet : answerSets) {
        for (const auto& word : curWordSet) {
            if(word == passed){
                foundVersions.push_back(word);
            }
        }
    }
    return foundVersions;
}
