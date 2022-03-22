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
        string currentRack;
        int wordXVal = 1;
        while(getline(rowStr, cell, ',')){
            if(x == 0)
                currentRack = cell;
            else if (x == 1)
                wordXVal = atoi(cell.c_str());
            else{
                TString TStr = cell;
                TStr.set_x_vals_to_subscripts();
                TStr.add_to_x_vals(wordXVal);
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

    ofstream outFile;
    outFile.open(printPath);
    if(!outFile.is_open())
        throw invalid_argument("Could not open " + printPath);

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
            outFile << sorted << ',' << TStr[0].x << ',' << curWord << endl;
            cout << sorted << ',' << TStr[0].x << ',' << curWord << endl;
            TStr.add_to_x_vals(1);
        }
    }
    cout << "RMAC::compute_and_print_rack_database(1):: " << count << " words read from ../Data/scrabble_word_list.txt" << endl;
    wordCorpus.close();
    subSevenOut.close();

    //------------------------------------------------------------
    cout << "RMAC::compute_and_print_rack_database(3.1):: Sub-rack Rebasing and Linking In-Progress" << endl;
    unordered_map<string, vector<TString>> rmCpy = *rackmap;
    for (int baseRackSize = 7; baseRackSize > 2; --baseRackSize) {
        for (const auto &i: rmCpy) {
            int rackLen = i.first.length();
            if (rackLen == baseRackSize - 1) {
                for (int j = 0; j < 26; ++j) {
                    string temp = i.first + char('A' + j);
                    for (const auto &k: (*rackmap)[i.first]) {
                        outFile << temp << ',' << k.read_at(0).x << ',' << k.to_string() << endl;
                    }
                }
            } else if (rackLen < baseRackSize) {
                ifstream rackAddOnFile;
                rackAddOnFile.open("../Data/Combinations" + to_string(baseRackSize - rackLen) + ".csv");
                if (!rackAddOnFile.is_open())
                    throw invalid_argument(
                            "Could not open ../Data/Combinations" + to_string(baseRackSize - rackLen) + ".csv");

                string rackAddOn;
                while (rackAddOnFile.good()) {
                    getline(rackAddOnFile, rackAddOn);
                    while (isspace(rackAddOn.back()))
                        rackAddOn.pop_back();

                    for (int j = 0; j < rackAddOn.length(); ++j) {
                        if(rackAddOn[j] == ',') {
                            rackAddOn.erase(j, 1);
                            j--;
                        }
                    }

                    string temp = i.first + rackAddOn;
                    for (const auto &k: (*rackmap)[i.first]) {
                        outFile << temp << ',' << k.read_at(0).x << ',' << k.to_string() << endl;
                        cout << temp << ',' << k.read_at(0).x << ',' << k.to_string() << endl;
                    }
                }
                rackAddOnFile.close();
            }
        }
        cout << "\tBase racks of size " << baseRackSize << " completed." << endl;
    }
    cout << "RMAC::compute_and_print_rack_database(3.2):: Sub-rack Rebasing and Linking Complete" << endl;

    throw invalid_argument("complete success");

    for (const auto& i : *rackmap) {
        for (const auto& j: i.second)
            outFile << i.first << ',' << j.read_at(0).x << ',' << j.to_string() << endl;
    }
    outFile.close();
    cout << "RMAC::compute_and_print_rack_database(4):: Rack-information printing successful. Printed to " << printPath << endl;

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