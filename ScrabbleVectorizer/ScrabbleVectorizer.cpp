#include "ScrabbleVectorizer.h"

void scl::ScrabbleVectorizer::search_for_intersecting_words() {
    //TODO: Remove as many if-statements as possible.
    // Convert AnchoredString into Anchoredsbl::Tstring, so that ScrabbleDataset is composed of pre-defined scl::TStrings.
    // Turn rackCount into a class/struct.
    // optimize solution as much as possible

    if(dictionary.empty())
        throw std::invalid_argument("Error in scl::ScrabbleVectorizer::search_for_intersecting_words() | The set of all scl words is empty.");

    int originalRackCount[28];
    int originalBlankCount = 0;
    for (int & i : originalRackCount) {
        i = 0;
    }
    for (unsigned char t : rack) {
        if(t == '?')
            originalBlankCount++;
        originalRackCount[t - 63]++;
    }

    int rowSubscript = 0;
    for (auto& row: board) {
        int tileCount = 0;
        for (auto& tile : row) {
            if(tile == ' ') {
                tileCount++;
                continue;
            }

            std::vector<AnchoredString> wordsOfTile = wordDataset.at_with(tileCount, tile.letter);
            for (const auto& word : wordsOfTile) {
                scl::Tstring curTStr;
                int anchorIndex = word.second;

                int blankCount = originalBlankCount;
                int rackCount[28];
                for (int i = 0; i < 28; ++i) {
                    rackCount[i] = originalRackCount[i];
                }

                bool skip = false;
                int start = 0 - anchorIndex;
                int end = word.first.length() - anchorIndex;
                for (int i = start; i < end; ++i) {
                    unsigned char curChar = word.first[i + anchorIndex];
                    scl::Tile curBoardTile = board[rowSubscript][tileCount + i];

                    if(curBoardTile == curChar) {
                        curTStr += scl::Tile(curChar, tileCount + i, rowSubscript, -1);
                    }
                    else if (curBoardTile != ' '){
                        skip = true;
                        break;
                    }
                    else{
                        curTStr += scl::Tile(curChar, tileCount + i, rowSubscript, -1);

                        if(rackCount[curChar - 63] == 0){
                            if(blankCount == 0){
                                skip = true;
                                break;
                            }
                            blankCount--;
                            curTStr.back().points = 0;
                            curTStr.back().isBlank = true;
                        }
                        else
                            rackCount[curChar - 63]--;
                    }
                }
                if(skip)
                    continue;

                moveSets[curTStr[0].y][curTStr[0].x].push_back(curTStr);    //problem: anchored index must be 0 for the word to be passed on
            }

            tileCount++;
        }
        rowSubscript++;
    }
}

void scl::ScrabbleVectorizer::reset_all_data() {
    bestX = bestY = 8;
    bestWord.clear();
    rack.clear();
    board.clear();
    dictionary.clear();
    dictionarySub8.clear();

    for (int i = 0; i < 15; ++i) {
        for (int j = 0; j < 15; ++j) {
            moveSets[i][j].clear();
        }
    }
}

void scl::ScrabbleVectorizer::place_into_board(const scl::Tstring &toPrint) {
    for (int i = toPrint.read_at(0).x; i < toPrint.length() + toPrint.read_at(0).x; i++) {
        if (board[toPrint.read_at(0).y][i] == ' ')
            board[toPrint.read_at(0).y][i] = scl::Tile(toPrint.read_at(i - toPrint.read_at(0).x).letter,
                                                       i,
                                                       toPrint.read_at(0).y, 1);
        perkBoard[toPrint.read_at(0).y][i] = ' ';
    }
}

bool scl::ScrabbleVectorizer::contains_letter_of_rack(const scl::Tstring& passed) const {
    std::unordered_set<char> handSet;
    for (char i : rack)
        handSet.emplace(toupper(i));

    for (int i = 0; i < passed.length(); ++i) {
        if (handSet.find(toupper(passed.read_at(i).letter)) != handSet.end()) {
            return true;
        }
    }
    return false;
}

std::vector<scl::Tstring> scl::ScrabbleVectorizer::return_raw_board_with(const scl::Tstring &toPrint) const {
    std::vector<scl::Tstring> boardCpy = board;
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

void scl::ScrabbleVectorizer::search_for_tangential_words() {    //does not support blank tiles
    //TODO: Remove as many if-statements as possible.
    // optimize solution as much as possible
    // Implement custom data-structure for tile placement checking for tangential words only (so you do not have to do over under explicitly).
    // optimize solution as much as possible
    if(routeRMAC == UNDEFINED_ROUTE)
        throw std::invalid_argument("Error in scl::ScrabbleVectorizer::search_for_tangential_words() | definition route for RMAC has not been set.");
    if(dictionary.empty())
        throw std::invalid_argument("Error in scl::ScrabbleVectorizer::search_for_tangential_words() | The set of all scl words is empty.");

    RMAC rackMAC;
    if(routeRMAC == DICTIONARY)
        rackMAC = RMAC(rack, dictionarySub8);
    else
        rackMAC = RMAC(rack, rmacFilePath);

    for (int i = 0; i < 15; ++i) {
        if(i - 1 > 0) {
            for (int j = 0; j < 15; ++j) {
                if (board[i][j] == ' ' || board[i - 1][j] != ' ')
                    continue;
                for (auto it: rackMAC.data) {
                    int start = it.read_at(0).x + j;
                    int end = start + it.length();
                    if (start < 0 || end > 15)
                        continue;

                    bool skip = false;
                    for (int k = start; k < end; ++k) {
                        if (board[i - 1][k] != ' ') {
                            skip = true;
                            break;
                        }
                        it[k - start].x = k;
                        it[k - start].y = i - 1;
                    }
                    if (skip)
                        continue;
                    moveSets[i - 1][it[0].x].push_back(it);
                }
            }
        }
        if(i + 1 < 15) {
            for (int j = 0; j < 15; ++j) {
                if (board[i][j] == ' ' || board[i + 1][j] != ' ')
                    continue;
                for (auto it: rackMAC.data) {
                    int start = it.read_at(0).x + j;
                    int end = start + it.length();
                    if (start < 0 || end > 15)
                        continue;

                    bool skip = false;
                    for (int k = start; k < end; ++k) {
                        if (board[i + 1][k] != ' ') {
                            skip = true;
                            break;
                        }
                        it[k - start].x = k;
                        it[k - start].y = i + 1;
                    }
                    if (skip)
                        continue;
                    moveSets[it[0].y][it[0].x].push_back(it);
                }
            }
        }
    }
}

void scl::ScrabbleVectorizer::clear_all_moves() {
    for (int i = 0; i < 15; ++i) {
        for (int j = 0; j < 15; ++j) {
            moveSets[i][j].clear();
        }
    }
}

int scl::ScrabbleVectorizer::points_of_placed_word(const scl::Tstring &word) const{
    // If a letter is shared between words, then count it's premium value for all words
    // Any word multiplier only gets assigned to the original word, and not any subsequently formed words
    // If a word is placed on two or more multiplier tiles, the words value is multiplied by both tile values
    // If a word uses all the tiles in the rack then 50 is added to the final total

    if(word.is_empty())
        return 0;

    int crossWordSum = 0;
    std::vector<scl::Tstring> boardCpy = return_raw_board_with(word);
    for (int i = 0; i < 15; ++i) {
        scl::Tstring column;
        for (int j = 0; j < 15; ++j) {
            column += boardCpy[j].read_at(i);
        }

        std::vector<scl::Tstring> colShards = column.fragments();

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

void scl::ScrabbleVectorizer::place_best_word_into_board() {
    for (int i = bestWord.read_at(0).x; i < bestWord.length() + bestWord.read_at(0).x; i++) {
        if (board[bestWord.read_at(0).y][i] == ' ')
            board[bestWord.read_at(0).y][i] = scl::Tile(bestWord.read_at(i - bestWord.read_at(0).x).letter,
                                                        i,
                                                        bestWord.read_at(0).y, 1);
        perkBoard[bestWord.read_at(0).y][i] = ' ';
    }
}

std::vector<std::string> scl::ScrabbleVectorizer::return_raw_perkBoard_copy() {
    std::vector<std::string> toReturn;
    for (int i = 0; i < 15; ++i) {
        std::string row;
        for (int j = 0; j < 15; ++j) {
            row += perkBoard[i][j];
        }
        toReturn.push_back(row);
    }
    return toReturn;
}

std::vector<std::string> scl::ScrabbleVectorizer::return_raw_char_board_copy() {
    std::vector<std::string> toReturn;
    for (int i = 0; i < 15; ++i) {
        std::string row;
        for (int j = 0; j < 15; ++j) {
            row += board[i][j].letter;
        }
        toReturn.push_back(row);
    }
    return toReturn;
}

void scl::ScrabbleVectorizer::build_dictionaries_from(const char* filePath) {
    std::ifstream dictionaryFile;
    dictionaryFile.open(filePath);
    if(!dictionaryFile.is_open())
        throw std::invalid_argument("could not open file passed to void scl::ScrabbleVectorizer::build_dictionaries_from(const char* filePath)");

    dictionary.clear();
    dictionarySub8.clear();
    std::string curWord;
    int count = 0;
    while(dictionaryFile.good()){
        getline(dictionaryFile, curWord);
        count++;

        while(isspace(curWord.back()))
            curWord.pop_back();

        dictionary.emplace(curWord);

        if(curWord.length() < 8)
            dictionarySub8.emplace(curWord);
    }
    std::cout << "scl::ScrabbleVectorizer:: " << count << " words read from " << filePath << std::endl;

    dictionaryFile.close();
}

scl::ScrabbleVectorizer::~ScrabbleVectorizer() {
    for (int i = 0; i < 15; ++i) {
        delete[] moveSets[i];
    }
    delete[] moveSets;
}

scl::ScrabbleVectorizer::ScrabbleVectorizer() {
    bestX = bestY = 8;

    moveSets = new std::vector<scl::Tstring>*[15];
    for (int i = 0; i < 15; ++i) {
        moveSets[i] = new std::vector<scl::Tstring>[15];
    }
}

scl::ScrabbleVectorizer::ScrabbleVectorizer(const std::string &passed) {
    bestX = bestY = 8;

    moveSets = new std::vector<scl::Tstring>*[15];
    for (int i = 0; i < 15; ++i) {
        moveSets[i] = new std::vector<scl::Tstring>[15];
    }

    rack = passed;
    sort(rack.begin(), rack.end());
}

void scl::ScrabbleVectorizer::prep_perkBoard() {
    for (int i = 0; i < 15; ++i) {
        for (int j = 0; j < 15; ++j) {
            if(board[i][j] != ' ')
                perkBoard[i][j] = ' ';
        }
    }
}
