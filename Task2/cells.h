//
// Created by Harmony on 15.03.2023.
//

#ifndef TASK2_CELLS_H
#define TASK2_CELLS_H
#include "vector"
#include <iostream>
#include <tuple>
#include <random>

enum class CellType {
    CROP,
    MOLE,
    COTTAGER,
    EMPTY
};

class Point{
public:
    int x;
    int y;
    Point(int y, int x);
    Point();

    bool operator<(const Point& loc2) const
    {
        return std::tie(this->x, this->y) < std::tie(loc2.x, loc2.y);;
    }

    operator std::string() const {
        return "Point: " + std::to_string(this->y) + " " + std::to_string(this->x);
    }

    friend std::ostream & operator <<(std::ostream &out, const Point &obj) {
        return out << static_cast<std::string>(obj);
    }
};

class Cell{
public:
    CellType cell_type = CellType::EMPTY;
    Cell();
    Cell(CellType cell_type);
};

class Mole{
private:
    static int get_random_number(int min, int max);
public:
    bool is_underground = true;
    bool is_mated = false;
    bool has_moved = false;
    bool has_actioned = false;
    int default_steps_until_hide;
    int steps_until_hide;
    bool sex;

    static Point want_move_to(int field_width, int field_height);
    void try_get_underground();

    Mole(bool sex, int default_steps_until_hide);
    Mole();
};

class Cottager{
public:
    int move_speed;
    int hit_radius;

    Cottager(int move_speed, int hit_radius);
    Cottager();
};

#endif //TASK2_CELLS_H
