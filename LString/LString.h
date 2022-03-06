



#ifndef SCRABBLE_SOLVER_LSTRING_H
#define SCRABBLE_SOLVER_LSTRING_H
#include <iostream>
#include <vector>
#include <fstream>
#include <unordered_map>
#include <map>
#include <unordered_set>
#include <algorithm>
#include <stdexcept>
#include <math.h>
#include <cstring>
#include <math.h>
using namespace std;

//const int map[26] = {101, 302, 303, 204, 105, 406, 207, 408, 109, 810, 511, 112, 313, 114, 115, 316, 1017, 118, 119, 120, 121, 422, 423, 824, 425, 1026};
//
//                       1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16  17 18 19 20 21 22 23 24 25  26
//                       a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p,  q, r, s, t, u, v, w, x, y,  z
const int legend[26] = { 1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10};

enum Type{HORIZONTAL, VERTICAL, UNDEFINED};

struct Letter {
    int x, y;
    int val;
    char LData;
    int flag;
//    const int legend[26] = { 1, 3, 3, 2, 1, 4, 2, 4, 1, 8,  5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10};

    Letter(){
        x = y = flag = -1;
        val = 0;
        LData = ' ';
    }

    Letter(char passed, int xp, int yp, int fl){
        LData = abs(passed);
        x = xp;
        y = yp;
        flag = fl;
        if((LData & 31) - 1 > 25 && LData != 32) {
            cout << '|' << char(toupper(LData)) << '|' << endl;
            throw invalid_argument("Error in Letter(char, int, int, int) | LData has an invalid value");
        }
        if(LData != ' ')
            val = legend[(LData & 31) - 1];
        else
            val = 0;
    }

    void setCoord(int xp, int yp){
        x = xp;
        y = yp;
    }

    bool operator==(const Letter& invoked) const{
        return (abs(invoked.LData) == abs(this->LData));
    }

    bool operator==(char invoked) const{
        return (abs(invoked) == abs(this->LData));
    }

    bool operator!=(const Letter& invoked) const{
        return (abs(invoked.LData) != abs(this->LData));
    }

    bool operator!=(char invoked) const{
        return (abs(invoked) != abs(this->LData));
    }

    Letter& operator=(const Letter& invoked){
        if(this == &invoked)
            return *this;
        LData = invoked.LData;
        y = invoked.y;
        x = invoked.x;
        flag = invoked.flag;
        if((LData & 31) - 1 > 25 && LData != 32) {
            cout << '|' << char(toupper(LData)) << '|' << endl;
            throw invalid_argument("Error in Letter(char, int, int, int) | LData has an invalid value");
        }
        if(LData != ' ')
            val = legend[(LData & 31) - 1];
        else
            val = 0;
        return *this;
    }

    Letter& operator=(char invoked){
        LData = abs(invoked);
        y = x = flag = -1;
        if((LData & 31) - 1 > 25 && LData != 32) {
            cout << '|' << char(toupper(LData)) << '|' << endl;
            throw invalid_argument("Error in Letter(char, int, int, int) | LData has an invalid value");
        }
        if(LData != ' ')
            val = legend[(LData & 31) - 1];
        else
            val = 0;
        return *this;
    }
};

class LString{
private:
    Letter* data = nullptr;
    int eleCount;
    int maxCap;
    
public:
    struct Iterator{
        Iterator(Letter* pssdDataPtr){dataPtr = pssdDataPtr;}

        Letter& operator*() const { return *dataPtr; }
        Iterator& operator++() { dataPtr++; return *this; }
        Iterator operator++(int) { Iterator temp = *this; dataPtr++; return temp; }
        Iterator& operator--() { dataPtr--; return *this; }
        Iterator operator--(int) { Iterator temp = *this; dataPtr--; return temp; }
        friend bool operator== (const Iterator& a, const Iterator& b) { return (a.dataPtr == b.dataPtr); };
        friend bool operator!= (const Iterator& a, const Iterator& b) { return (a.dataPtr != b.dataPtr); };

        Letter* dataPtr;
    };
    Iterator begin() { return Iterator(data); }
    Iterator end()   { return Iterator(data + eleCount); }



    LString();
    LString(const LString&);
    LString(const string&);
    LString(char *);
    LString& operator=(const LString&);
    LString& operator=(const string&);
    LString& operator=(char*);

    Letter& operator[](int);
    Letter read_at(int) const;
    int length() const;
    int read_maxCap() const;
    Letter read_back() const;
    Letter& back();
    int find_pos_of(Letter) const;

    LString& push_back(Letter);
    LString& push_front(Letter);
    LString& push_back(char);
    LString& push_front(char);

    LString& pop_back();
    LString& pop_front();
    LString& clear();
    LString& erase_at(int);

    bool is_empty() const;
    bool contains(Letter) const;
    bool contains(char) const;
    bool contains(string) const;
    bool contains(LString) const;
    bool containsIgnorePadding(LString) const;
    bool operator==(const LString&) const;
    LString& operator+=(const LString&);
    LString& operator+=(const Letter&);
    LString& operator+=(char);
    LString operator+(const LString&) const;
    LString operator+(const Letter&) const;
    LString operator+(char) const;
    string to_string() const;
    bool is_descendent_of(const LString&, const LString&);
    bool is_descendent_of(const LString&);
    int get_letter_points() const;
    static int get_horizontal_points(string);
    LString& xVals_to_subscript();
    LString& add_to_x_vals(int);
    LString& set_x_vals_equal_to(int);
    LString& set_y_vals_equal_to(int);
    bool contains_flag(int) const;
    vector<LString> break_into_frags() const;
    bool is_blank_LStr() const;
    
    ~LString(){delete[] data;}

protected:
    LString(int);
    void increaseMaxCapacity();
};

class MyHashFunction {
public:
    size_t operator()(const LString& lstr) const{
        unsigned long hash = 5381;

        for (int i = 0; i < lstr.length(); ++i) {
            hash = ((hash << 5) + hash) + abs(lstr.read_at(i).LData); /* hash * 33 + c */
        }

        return hash;
    }
};

#endif
