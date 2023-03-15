//
// Created by Harmony on 15.03.2023.
//

#include "cells.h"


CellType GameObject::get_type() {
    return object_type;
}

GameObject::GameObject() {
    this->object_type = CellType::EMPTY;
}

Mole::Mole(bool sex, int default_steps_until_hide) {
    this->default_steps_until_hide = default_steps_until_hide;
    this->steps_until_hide = default_steps_until_hide;
    this->sex = sex;
    this->object_type = CellType::MOLE;
}

int Mole::get_random_number(int min, int max) {
    // Установить генератор случайных чисел
    srand(time(nullptr));

    // Получить случайное число - формула
    int num = min + rand() % (max - min + 1);

    return num;
}

Point::Point(int x, int y) {
    this->x = x;
    this->y = y;
}

Point Mole::want_move_to(int field_width, int field_height) {
    int x = get_random_number(0, field_width);
    int y = get_random_number(0, field_height);
    return *new Point(x, y);
}

void Mole::try_get_underground() {
    if (this->steps_until_hide > 0){
        this->steps_until_hide = this->steps_until_hide - 1;
    } else {
        this->is_underground = true;
    }
}

Cottager::Cottager(int move_speed, int hit_radius) {
    this->move_speed = move_speed;
    this->hit_radius = hit_radius;
    this->object_type = CellType::COTTAGER;
}
