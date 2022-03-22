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
    data = new unordered_map<string, vector<TString>>();

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
        int wordXVal = 1;
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
                    wordXVal = atoi(cell.c_str());
                }
            }
            else{
                TString TStr = cell;
                TStr.set_x_vals_to_subscripts();
                TStr.add_to_x_vals(wordXVal);
                if(currentRack == "ADEGOSS")
                    cout << "RRFF" << endl;
                (*data)[currentRack].emplace_back(TStr);
            }

            x++;
        }
    }
    rackDatabase.close();
    cout << "RMAC::RMAC():: " << count << " rack-information sets read from ../Data/RackDatabase.csv" << endl;
}

void RMAC::compute_and_print_rack_database(string wordCorpusPath, string printPath) {
    auto* rackmap = new unordered_map<string, vector<TString>>();

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
        int end = TStr.length();
        TStr.set_x_vals_to_subscripts();
        TStr.add_to_x_vals(1 - end);
        while(TStr.back().x < end){
            (*rackmap)[sorted].emplace_back(TStr);
            TStr.add_to_x_vals(1);
        }
    }
    cout << "RMAC::compute_and_print_rack_database(1):: " << count << " words read from ../Data/scrabble_word_list.txt" << endl;
    wordCorpus.close();
    subSevenOut.close();

    //------------------------------------------------------------

    cout << "RMAC::compute_and_print_rack_database(2):: Raw-sub-rack progress" << endl;
    for (int i = 3; i < 8; ++i) {
        cout << "\t" << i << " - ";
        wordCorpus.open(wordCorpusPath);
        if(!wordCorpus.is_open())
            throw invalid_argument("Could not open " + wordCorpusPath);

        char progressFlag = '<';
        curWord.clear();
        string curSubSevenWord;
        while (wordCorpus.good()) {
            getline(wordCorpus, curWord);
            while (isspace(curWord.back()))
                curWord.pop_back();

            if (curWord.length() != i || curWord.empty())
                continue;

            if(progressFlag != curWord[0]) {
                progressFlag = curWord[0];
                cout << progressFlag << ' ';
            }

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
                if(TStr.length() >= i || !TStr.is_descendent_of(curWord))
                    continue;

                int end = TStr.length();
                TStr.set_x_vals_to_subscripts();
                TStr.add_to_x_vals(1 - end);
                while(TStr.back().x < end){
                    (*rackmap)[sorted].emplace_back(TStr);
                    TStr.add_to_x_vals(1);
                }
            }
            wordsSubSeven.close();
        }
        wordCorpus.close();
        cout << endl;
    }
    cout << "RMAC::compute_and_print_rack_database(2):: Raw-sub-rack combination successful" << endl;
//    cout << "RMAC::compute_and_print_rack_database(3):: Given-rack/sub-rack relationship accounting successful" << endl;

    ofstream outFile;
    outFile.open(printPath);
    if(!outFile.is_open())
        throw invalid_argument("Could not open " + printPath);
    for (const auto& i : *rackmap) {
        outFile << "rack," << i.first << endl;
        for (const auto& j: i.second)
            outFile << "word," << j.read_at(0).x << ',' << j.to_string() << endl;
    }
    outFile.close();
    cout << "RMAC::compute_and_print_rack_database(3):: Rack-information printing successful. Printed to " << printPath << endl;

    throw invalid_argument("complete success");
}

vector<TString> &RMAC::operator[](const string& key) {
    if(key.length() > 7)
        throw invalid_argument("Error in vector<AnchoredString> &RMAC::return_this_at(int, string) | Invalid "
                               "parameter value.\nKey Length: " + to_string(key.length()));
    string sorted = key;
    sort(sorted.begin(), sorted.end());

    for(auto& it : (*data)[sorted]){
        cout << it.to_string() << endl;
    }
    return (*data)[sorted];
}
