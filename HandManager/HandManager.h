//
// Created by misc1 on 2/26/2022.
//

#ifndef SCRABBLEBOT_HANDMANAGER_H
#define SCRABBLEBOT_HANDMANAGER_H


#include <iostream>
#include <vector>
#include <fstream>
#include <unordered_map>
#include <algorithm>
#include "../LString/LString.h"
#include <math.h>
using namespace std;

/*
 To do:
 * make program check each open letter for longest possible word at that letter
 * For each letter with a word size greater than 1: make program compute all clean answers for all possible r-combinations of the given set of letters, then all power sets of each combination, then all permutations of each power set element. where r is the largest word
 * Add each computed cleanAnswer word to a grand list
 * run through this list for the word with the greatest points, if there are multiple words that tie for the greatest amount of point, use the word with the least amount of letters
 */

const int map[26] = {101, 302, 303, 204, 105, 406, 207, 408, 109, 810, 511, 112, 313, 114, 115, 316, 1017, 118, 119, 120, 121, 422, 423, 824, 425, 1026};

//  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26
//  a, b, c, d, e, f, g, h, i, j,  k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z
const int legend[26] = { 1, 3, 3, 2, 1, 4, 2, 4, 1, 8,  5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10};

class MyHashFucntion {
public:
    size_t operator()(const string & str) const{
        size_t hash = 0;
        size_t i = 0;
        size_t len = str.length();

        for (i = 0; i < len; i++) {
            hash = (str[i]) + (hash << 6) + (hash << 16) - hash;
        }

        return hash;
    }
};

class HandManager{
public:
    HandManager();
    HandManager(string hand);
    void StartPermute();
    void Permute(string a, int l, int r);
    void SortManager();
    void sort(int *arr, int low, int high);
    void merge(int *arr, int low, int high, int mid);
    void PowerSet();
    void toString() const;
    void CleanPossibleAnswers();
    void PromptFile(string directory);
    void SetSizeLimit(int passed){sizeLimit = passed;}
    int GetSizeLimit() const {return sizeLimit;}
    string GetBestWord(){
        if(cleanAnswers.empty())
            throw invalid_argument("Error in GetBestWord | There are no clean answers.");
        return cleanAnswers[0];}
    string GetBestWord(int);
    string GetBestWord(int, char, int);
    static int gradeWord(string);
    string GetBestWord(int, string, int);   //get the best word that contains a string that has left padding of size int1, and right padding of size int2
    string GetBestWord(LString);             //pass a string a letters, fill the string with every possible answer one by one, only overwritting spaces; then clean possible answers and return first word in possible answers

private:
    string hand;
    vector<string> possibleAnswers;
    vector<string> handPowerSet;
    vector<string> cleanAnswers;
    int sizeLimit;
};

#endif //SCRABBLEBOT_HANDMANAGER_H
