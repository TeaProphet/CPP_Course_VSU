//
// Created by Harmony on 15.03.2023.
//

#ifndef TASK2_GAME_H
#define TASK2_GAME_H
#include "cells.h"
#include "GameField.h"
#include <iostream>
#include <map>
#include <cmath>
#include <windows.h>


class Game {
private:
    GameField field;
    std::map<Point, Mole> moles_map;
    Point cottager_position;
    Cottager cottager;
    int crop_num;
public:
    Game(int field_width, int field_height, int moles_count, int move_speed, int hit_radius);
    void start();
    void generate_crop();

    void mole_action(Point mole_coords, Mole& mole);
    bool find_mate(Point mole_coords, Mole& mole);
    void generate_mole();
    bool check_type(Point point, CellType cell_type);
    void harvest_crop(Point mole_coords);

    Point find_nearest_mole();
    void move_cottager_to(Point move_to);
    void kill_mole(Point mole_pos);
};


#endif //TASK2_GAME_H
