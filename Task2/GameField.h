//
// Created by Harmony on 16.03.2023.
//

#ifndef TASK2_GAMEFIELD_H
#define TASK2_GAMEFIELD_H
#include "cells.h"
#include <map>


class GameField {
private:
    Cell **field;
public:
    int x_size;
    int y_size;
    GameField(int x_size, int y_size, Cell **field);
    GameField();

    Cell get(int y, int x);
    void set(int y, int x, Cell cell);
    bool is_inside_field(int y, int x) const;

    void print_field(std::map<Point, Mole> mole_map);
};


#endif //TASK2_GAMEFIELD_H
