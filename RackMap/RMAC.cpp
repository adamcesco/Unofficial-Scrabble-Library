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