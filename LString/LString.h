



#ifndef SCRABBLE_SOLVER_LSTRING_H
#define SCRABBLE_SOLVER_LSTRING_H
#include <iostream>
#include <vector>
#include <fstream>
#include <unordered_map>
#include <algorithm>
#include <stdexcept>
#include <math.h>
using namespace std;

struct Letter {
    int x, y;
    int val;
    char LData;
    const int legend[26] = { 1, 3, 3, 2, 1, 4, 2, 4, 1, 8,  5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10};

    Letter(){
        x = y = -1;
        val = 0;
        LData = ' ';
    }

    Letter(char passed, int xp, int yp){
        LData = passed;
        x = xp;
        y = yp;
        val = legend[(passed & 31) - 1];
    }

    void setCoord(int xp, int yp){
        x = xp;
        y = yp;
    }

    bool operator==(const Letter& invoked) const{
        return (invoked.LData == this->LData);
    }

    bool operator==(char invoked) const{
        return (invoked == this->LData);
    }

    Letter& operator=(const Letter& invoked){
        LData = invoked.LData;
        y = invoked.y;
        x = invoked.x;
        val = legend[(LData & 31) - 1];
        return *this;
    }

    Letter& operator=(char invoked){
        LData = invoked;
        y = -1;
        x = -1;
        val = legend[(LData & 31) - 1];
        return *this;
    }
};

class LString{
private:
    Letter* data;
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
    LString& operator=(const LString&);
    LString& operator=(const string&);

    Letter& operator[](int);
    Letter read_at(int) const;
    int size() const;
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
    bool operator==(const LString&) const;
    LString& operator+=(const Letter&);
    LString& operator+=(char);
    string to_string();
    
    ~LString(){delete[] data;}

protected:
    LString(int);
    void increaseMaxCapacity();
};




#endif