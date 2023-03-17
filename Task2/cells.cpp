//
// Created by Harmony on 15.03.2023.
//

#include "cells.h"

using namespace std;

Mole::Mole(bool sex, int default_steps_until_hide) {
    this->default_steps_until_hide = default_steps_until_hide;
    this->steps_until_hide = default_steps_until_hide;
    this->sex = sex;
}

int Mole::get_random_number(int min, int max) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist(min, max);
    return dist(gen);
}

Point::Point(int y, int x) {
    this->x = x;
    this->y = y;
}

Point::Point() {

}

Point Mole::want_move_to(int field_width, int field_height) {
    int x = get_random_number(0, field_width - 1);
    int y = get_random_number(0, field_height - 1);
    return *new Point(x, y);
}

void Mole::try_get_underground() {
    if (this->steps_until_hide > 0){
        this->steps_until_hide = this->steps_until_hide - 1;
    } else {
        this->is_underground = true;
        steps_until_hide = default_steps_until_hide;
        has_moved = false;
    }
}

Mole::Mole() {

}

Cottager::Cottager(int move_speed, int hit_radius) {
    this->move_speed = move_speed;
    this->hit_radius = hit_radius;
}

Cottager::Cottager() {

}

Cell::Cell(CellType cell_type) : cell_type(cell_type){
}

Cell::Cell() {

}
