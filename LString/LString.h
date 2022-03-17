



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
#include <cstring>
#include <cmath>
using namespace std;

//                       1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16  17 18 19 20 21 22 23 24 25  26
//                       a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p,  q, r, s, t, u, v, w, x, y,  z
const int legend[26] = { 1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10};

struct Letter {
    char LData;     //think of the LData as the face value of the Letter object, with all other data members being the Letter objects description
    int x, y;
    int val;
    int flag;
    bool isBlank = false;

    Letter(){
        x = y = flag = -1;
        val = 0;
        LData = ' ';
        isBlank = false;
    }

    Letter(char passed, int xp, int yp, int fl){
        LData = abs(passed);
        x = xp;
        y = yp;
        flag = fl;
        isBlank = false;

        if(isalpha(LData))
            val = legend[(LData & 31) - 1];
        else
            val = 0;
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
        val = invoked.val;
        isBlank = invoked.isBlank;
        return *this;
    }

    Letter& operator=(char invoked){
        LData = abs(invoked);
        y = x = flag = -1;
        isBlank = false;

        if(isalpha(LData))
            val = legend[(LData & 31) - 1];
        else
            val = 0;
        return *this;
    }
};

class LString{
private:
    Letter data[46];
    int eleCount = 0;

public:
    struct Iterator{
        Iterator(Letter* pssdDataPtr){ dataPtr = pssdDataPtr; }

        Letter& operator*() const{ return *dataPtr; }
        Iterator& operator++()   { dataPtr++; return *this; }
        Iterator operator++(int) { Iterator temp = *this; dataPtr++; return temp; }
        Iterator& operator--()   { dataPtr--; return *this; }
        Iterator operator--(int) { Iterator temp = *this; dataPtr--; return temp; }
        friend bool operator== (const Iterator& a, const Iterator& b){ return (a.dataPtr == b.dataPtr); };
        friend bool operator!= (const Iterator& a, const Iterator& b){ return (a.dataPtr != b.dataPtr); };

        Letter* dataPtr;
    };
    Iterator begin() { return Iterator(data); }
    Iterator end()   { return Iterator(data + eleCount); }

    //----------------------------------------------------------------------

    LString(){ eleCount = 0; }
    LString(const LString&);
    LString(const string&);
    LString(char *);
    LString& operator=(const LString&);
    LString& operator=(const string&);
    LString& operator=(char*);

    Letter& operator[](int);
    Letter read_at(int) const;
    int length() const{ return eleCount; }
    static int max_capacity() { return 45; }
    Letter read_back() const;
    Letter& back();
    int find_pos_of(char) const;

    LString& push_back(Letter);
    LString& push_back(char);

    LString& pop_back();
    LString& clear(){ eleCount = 0; return *this; }
    LString& erase_at(int);     //needs testing

    bool is_empty() const{ if(eleCount > 0){ return false; } return true; }
    bool contains(Letter) const;
    bool contains(char) const;
    bool operator==(const LString&) const;
    LString& operator+=(const Letter&);
    LString& operator+=(char);
    LString operator+(const Letter&) const;
    LString operator+(char) const;
    string to_string() const;

    //Methods below are unique to LString
    bool row_is_descendent_of(const string&, const LString&, LString&);
    bool is_descendent_of(const string&);
    int get_letter_points() const;
    static int get_letter_points(string);
    LString& set_x_vals_to_subscripts();
    LString& add_to_x_vals(int);
    LString& set_x_vals_equal_to(int);
    LString& set_y_vals_equal_to(int);
    bool contains_flag(int) const;
    vector<LString> fragments() const;
    bool is_all_whitespace() const;
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
