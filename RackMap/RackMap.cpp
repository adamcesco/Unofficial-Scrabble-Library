#include "RackMap.h"

RackMap::RackMap() {
    data = new vector<TString>**[15];
    for (int i = 0; i < 15; ++i) {
        data[i] = new vector<TString>*[8];
        for (int j = 0; j < 8; ++j){
            data[i][j] = new vector<TString>[198];
            for (int k = 0; k < 198; ++k)
                data[i][j][k] = vector<TString>();
        }
    }

    ifstream englishWords;
    englishWords.open("../Data/scrabble_word_list.txt");
    if(!englishWords.is_open())
        throw invalid_argument("Could not open ../Data/scrabble_word_list.txt");

    string curWord;
    int count = 0;
    while(englishWords.good()){
        getline(englishWords, curWord);
        count++;
        while(isspace(curWord.back()))
            curWord.pop_back();

        if(curWord.length() > 7)
            continue;

        for (int i = 0; i < 15; ++i) {
            TString TStr = curWord;
            TStr.set_x_vals_to_subscripts();
            int start = i - curWord.length();
            while(start < 0)
                start++;
            TStr.add_to_x_vals(start);

            int end = i + curWord.length();
            int sum = 0;
            for (char j : curWord)
                sum += int(abs(j) & 31);
            while(start < end && start < 15){
                data[i][curWord.length()][sum].emplace_back(TStr);
                TStr.add_to_x_vals(1);
                start++;
            }
        }
    }
    cout << "RackMap:: " << count << " words read from ../Data/scrabble_word_list.txt" << endl;

    englishWords.close();
}

vector<TString>& RackMap::return_this_at(int x, const string& key) {
    if(x > 14 || key.length() > 7)
        throw invalid_argument("Error in vector<AnchoredString> &RackMap::return_this_at(int, string) | Invalid "
                               "parameter value.\nX: " + to_string(x) + "\nKey Length: " + to_string(key.length()));
    int sum = 0;
    for (char i : key)
        sum += int(abs(i) & 31);
    if(sum > 198)
        throw invalid_argument("Error in vector<AnchoredString> &RackMap::return_this_at(int, string) | Invalid "
                               "parameter value.\nKey: " + key + "\nHash Value" + to_string(sum));
    return data[x][key.length()][sum];
}

RackMap::~RackMap() {
    for (int i = 0; i < 15; ++i) {
        for (int j = 0; j < 8; ++j)
            delete[] data[i][j];
        delete[] data[i];
    }
    delete[] data;
}
