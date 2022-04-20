#ifndef SCRABBLE_SOLVER_TSTRING_H
#define SCRABBLE_SOLVER_TSTRING_H

#include "Tile.h"
#include <string>
#include <vector>

namespace scl {
    class Tstring {
    private:
        Tile data[20];
        int eleCount = 0;

    public:
        struct Iterator {
            Iterator(Tile *pssdDataPtr) { dataPtr = pssdDataPtr; }

            Tile &operator*() const { return *dataPtr; }

            Iterator &operator++() {
                dataPtr++;
                return *this;
            }

            Iterator operator++(int) {
                Iterator temp = *this;
                dataPtr++;
                return temp;
            }

            Iterator &operator--() {
                dataPtr--;
                return *this;
            }

            Iterator operator--(int) {
                Iterator temp = *this;
                dataPtr--;
                return temp;
            }

            friend bool operator==(const Iterator &a, const Iterator &b) { return (a.dataPtr == b.dataPtr); };

            friend bool operator!=(const Iterator &a, const Iterator &b) { return (a.dataPtr != b.dataPtr); };

            Tile *dataPtr;
        };

        Iterator begin() { return Iterator(data); }

        Iterator end() { return Iterator(data + eleCount); }

        //----------------------------------------------------------------------

        Tstring() { eleCount = 0; }

        Tstring(const Tstring &);

        Tstring(std::string);

        Tstring(const char *);

        Tstring &operator=(const Tstring &);

        Tstring &operator=(const std::string &);

        Tstring &operator=(const char *);

        Tile &operator[](int);

        Tile read_at(int) const;

        inline int length() const { return eleCount; }

        static int max_capacity() { return 20; }

        Tile read_back() const;

        Tile &back();

        int find_pos_of(char) const;

        Tstring &pop_back();

        inline Tstring &clear() {
            eleCount = 0;
            return *this;
        }

        Tstring &erase_at(int);

        inline bool is_empty() const {
            if (eleCount > 0) { return false; }
            return true;
        }

        virtual bool contains(Tile) const;

        bool contains(char) const;

        virtual bool operator==(const Tstring &) const;

        bool operator==(const std::string &) const;

        bool operator==(const char *) const;

        Tstring &operator+=(const Tile &);

        Tstring &operator+=(char);

        Tstring operator+(const Tile &) const;

        Tstring operator+(char) const;

        //----------------------------------------------------------------------
        //Methods below are unique to Tstring:

        virtual std::string to_string() const;

        bool row_is_descendent_of(const std::string &, const Tstring &, Tstring &);

        bool is_descendent_of(const std::string &);

        int get_letter_points() const;

        static int get_letter_points(std::string);

        Tstring &set_x_vals_to_subscripts();

        Tstring &add_to_x_vals(int);

        Tstring &set_x_vals_equal_to(int);

        Tstring &set_y_vals_equal_to(int);

        bool contains_flag(int) const;

        std::vector<Tstring> fragments() const;

        std::vector<std::string> string_fragments() const;

        bool is_all_whitespace() const;
    };
}
#endif
