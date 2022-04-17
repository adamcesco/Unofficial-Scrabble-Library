#ifndef SCRABBLE_SOLVER_CADS_H
#define SCRABBLE_SOLVER_CADS_H

#include "../Tstring/Tstring.h"

namespace ssl{
    struct AnchoredString {
        AnchoredString(const std::string &str1, int subscript) {
            first = str1;
            second = subscript;
        }

        std::string first;
        int second;
    };

    class CADS {    //Coordinated Anchored Data Set
    public:
        CADS() { data = nullptr; }

        explicit CADS(const char *);

        CADS(const CADS &);

        CADS &operator=(const CADS &);

        std::vector<AnchoredString> &at_with(int x, unsigned char toFind);

        CADS &clear_all();

        ~CADS();

    private:
        std::vector<AnchoredString> **data;
        //TODO: You could implement a CADS that is based off of Anchored-TStrings instead of AnchoredStrings
        //TODO: You could implement a CADS that is based off of Anchored-DAWGs instead of AnchoredStrings
    };

}


#endif //SCRABBLE_SOLVER_CADS_H
