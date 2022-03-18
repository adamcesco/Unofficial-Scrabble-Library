#ifndef SCRABBLE_SOLVER_TSTRING_H
#define SCRABBLE_SOLVER_TSTRING_H

#include "Tile.h"
using namespace std;

class TString{
private:
    Tile data[46];
    int eleCount = 0;

public:
    struct Iterator{
        Iterator(Tile* pssdDataPtr){ dataPtr = pssdDataPtr; }

        Tile& operator*() const{ return *dataPtr; }
        Iterator& operator++()   { dataPtr++; return *this; }
        Iterator operator++(int) { Iterator temp = *this; dataPtr++; return temp; }
        Iterator& operator--()   { dataPtr--; return *this; }
        Iterator operator--(int) { Iterator temp = *this; dataPtr--; return temp; }
        friend bool operator== (const Iterator& a, const Iterator& b){ return (a.dataPtr == b.dataPtr); };
        friend bool operator!= (const Iterator& a, const Iterator& b){ return (a.dataPtr != b.dataPtr); };

        Tile* dataPtr;
    };
    Iterator begin() { return Iterator(data); }
    Iterator end()   { return Iterator(data + eleCount); }

    //----------------------------------------------------------------------

    TString(){ eleCount = 0; }
    TString(const TString&);
    TString(string);
    TString(const char *);
    TString& operator=(const TString&);
    TString& operator=(const string&);
    TString& operator=(const char*);

    Tile& operator[](int);
    Tile read_at(int) const;
    inline int length() const{ return eleCount; }
    static int max_capacity() { return 45; }
    Tile read_back() const;
    Tile& back();
    int find_pos_of(char) const;

    TString& pop_back();
    inline TString& clear(){ eleCount = 0; return *this; }
    TString& erase_at(int);

    inline bool is_empty() const{ if(eleCount > 0){ return false; } return true; }
    virtual bool contains(Tile) const;
    bool contains(char) const;
    virtual bool operator==(const TString&) const;
    bool operator==(const string&) const;
    bool operator==(const char*) const;
    TString& operator+=(const Tile&);
    TString& operator+=(char);
    TString operator+(const Tile&) const;
    TString operator+(char) const;
    virtual string to_string() const;

    //Methods below are unique to TString
    bool row_is_descendent_of(const string&, const TString&, TString&);
    bool is_descendent_of(const string&);
    int get_letter_points() const;
    static int get_letter_points(string);
    TString& set_x_vals_to_subscripts();
    TString& add_to_x_vals(int);
    TString& set_x_vals_equal_to(int);
    TString& set_y_vals_equal_to(int);
    bool contains_flag(int) const;
    vector<TString> fragments() const;
    vector<string> string_fragments() const;
    bool is_all_whitespace() const;
};

#endif
