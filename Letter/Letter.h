//
// Created by misc1 on 2/27/2022.
//

#ifndef SCRABBLE_SOLVER_LETTER_H
#define SCRABBLE_SOLVER_LETTER_H


struct Letter {
    int x, y;
    char data;

    Letter(char passed){
        data = passed;
    }

    void setCoord(int xp, int yp){
        x = xp;
        y = yp;
    }
};


#endif //SCRABBLE_SOLVER_LETTER_H
