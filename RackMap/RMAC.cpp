#include "RMAC.h"

void printArray(int size, int* arr){
    ofstream outFile;
    string filePath = "../Data/size_" + to_string(size) + "_combinations.csv";
    outFile.open(filePath, ios::app);
    if(!outFile.is_open())
        throw invalid_argument("Could not open ../Data/scrabble_word_list.txt");

    for (int i = 0; i < size; ++i) {
        outFile << char(arr[i]);
    }
    outFile << endl;
    outFile.close();
}

RMAC::RMAC() {
    data = new unordered_map<string, vector<TString>>*[15];
    for (int i = 0; i < 15; ++i) {
        data[i] = new unordered_map<string, vector<TString>>[8];
        for (int j = 0; j < 8; ++j){
            data[i][j] = unordered_map<string, vector<TString>>();
        }
    }

    ifstream rackDatabase;
    rackDatabase.open("../Data/RackDatabase.csv");
    if(!rackDatabase.is_open())
        throw invalid_argument("Could not open ../Data/RackDatabase.csv");

    string row;
    int count = 0;
    while(rackDatabase.good()) {
        getline(rackDatabase, row);

        int x = 0;
        string cell;
        stringstream rowStr(row);
        bool rack = false;
        string currentRack;
        int wordXVal = -1;
        while(getline(rowStr, cell, ',')){
            if(x == 0){
                if(cell[0] == 'r'){
                    rack = true;
                    count++;
                }
                else{
                    rack = false;
                }
            }
            else if (x == 1){
                if(rack){
                    currentRack = cell;
                }
                else{
                    wordXVal = abs(cell[0]) & 15;
                }
            }
            else{
                data[wordXVal][currentRack.length()][currentRack].emplace_back(cell);
            }

            x++;
        }
    }
    rackDatabase.close();
    cout << "RMAC::RMAC():: " << count << " rack-information sets read from ../Data/scrabble_word_list.txt" << endl;
}

vector<TString>& RMAC::return_this_at(int x, const string& key) {
    if(x > 14 || key.length() > 7)
        throw invalid_argument("Error in vector<AnchoredString> &RMAC::return_this_at(int, string) | Invalid "
                               "parameter value.\nX: " + to_string(x) + "\nKey Length: " + to_string(key.length()));
    int sum = 0;
    for (char i : key)
        sum += int(abs(i) & 31);
    if(sum > 198)
        throw invalid_argument("Error in vector<AnchoredString> &RMAC::return_this_at(int, string) | Invalid "
                               "parameter value.\nKey: " + key + "\nHash Value" + to_string(sum));
    string sorted = key;
    sort(sorted.begin(), sorted.end());

    for(auto& it : data[x][key.length()][sorted]){
        cout << it.to_string() << endl;
    }
    return data[x][key.length()][sorted];
}

RMAC::~RMAC() {
    for (int i = 0; i < 15; ++i) {
        delete[] data[i];
    }
    delete[] data;
}

void RMAC::compute_and_print_rack_database(string wordCorpusPath, string printPath) {   //todo: fix
    unordered_map<string, vector<TString>>** rackmap = nullptr;
    rackmap = new unordered_map<string, vector<TString>>*[15];
    for (int i = 0; i < 15; ++i) {
        rackmap[i] = new unordered_map<string, vector<TString>>[8];
        for (int j = 0; j < 8; ++j){
            rackmap[i][j] = unordered_map<string, vector<TString>>();
        }
    }

    ifstream wordCorpus;
    wordCorpus.open(wordCorpusPath);
    if(!wordCorpus.is_open())
        throw invalid_argument("Could not open " + wordCorpusPath);

    ofstream subSevenOut;
    subSevenOut.open("../Data/words_length_sub_7.txt");
    if(!subSevenOut.is_open())
        throw invalid_argument("Could not open ../Data/words_length_sub_7.txt");

    string curWord;
    int count = 0;
    while(wordCorpus.good()){
        getline(wordCorpus, curWord);
        count++;
        while(isspace(curWord.back()))
            curWord.pop_back();

        if(curWord.length() > 7 || curWord.empty())
            continue;
        else if(curWord.length() < 7)
            subSevenOut << curWord << endl;

        string sorted = curWord;
        sort(sorted.begin(), sorted.end());
        TString TStr = curWord;
        for (int i = 0; i < 15; ++i) {
            TStr.set_x_vals_to_subscripts();
            int start = int(i - curWord.length()) + 1;
            if(start < 0)
                start = 0;
            TStr.add_to_x_vals(start);

            int end = i + curWord.length();
            if(end > 15)
                end = 15;

            while(TStr.back().x < end){
                rackmap[i][curWord.length()][sorted].emplace_back(TStr);
                TStr.add_to_x_vals(1);
                start++;
            }
        }
    }
    cout << "RMAC::compute_and_print_rack_database(1):: " << count << " words read from ../Data/scrabble_word_list.txt" << endl;
    wordCorpus.close();
    subSevenOut.close();

    //------------------------------------------------------------

    for (int h = 3; h < 8; ++h) {
        wordCorpus.open(wordCorpusPath);
        if(!wordCorpus.is_open())
            throw invalid_argument("Could not open " + wordCorpusPath);

        curWord.clear();
        string curSubSevenWord;
        while (wordCorpus.good()) {
            getline(wordCorpus, curWord);
            while (isspace(curWord.back()))
                curWord.pop_back();

            if (curWord.length() != h || curWord.empty())
                continue;

            string sorted = curWord;
            sort(sorted.begin(), sorted.end());

            ifstream wordsSubSeven;
            wordsSubSeven.open("../Data/words_length_sub_7.txt");
            if (!wordsSubSeven.is_open())
                throw invalid_argument("Could not open words_length_sub_7.txt");

            while (wordsSubSeven.good()) {
                getline(wordsSubSeven, curSubSevenWord);
                while (isspace(curSubSevenWord.back()))
                    curSubSevenWord.pop_back();

                TString TStr = curSubSevenWord;
                if(!TStr.is_descendent_of(curWord))
                    continue;

                for (int i = 0; i < 15; ++i) {
                    TStr.set_x_vals_to_subscripts();

                    int start = int(i - TStr.length()) + 1;
                    if (start < 0)
                        start = 0;
                    TStr.add_to_x_vals(start);

                    int end = i + TStr.length();
                    if (end > 15)
                        end = 15;

                    while (TStr.back().x < end) {
                        rackmap[i][sorted.length()][sorted].emplace_back(TStr);
                        TStr.add_to_x_vals(1);
                        start++;
                    }
                }
            }
            wordsSubSeven.close();
        }
        wordCorpus.close();
    }
    cout << "RMAC::compute_and_print_rack_database(2):: Raw-sub-rack combination successful" << endl;
//    cout << "RMAC::compute_and_print_rack_database(3):: Given-rack/sub-rack relationship accounting successful" << endl;

    ofstream outFile;
    outFile.open(printPath);
    if(!outFile.is_open())
        throw invalid_argument("Could not open " + printPath);

    for (int i = 0; i < 15; ++i) {
        for (int j = 2; j < 8; ++j) {
            for (const auto& k : rackmap[i][j]) {
                outFile << "rack," << k.first << endl;
                for (const auto& l: k.second) {
                    outFile << "word," << i << ',' << l.to_string() << endl;
                }
            }
        }
    }
    outFile.close();
    cout << "RMAC::compute_and_print_rack_database(3):: Rack-information printing successful. Printed to " << printPath << endl;

    throw invalid_argument("complete success");
}
