//
// Created by Harmony on 15.03.2023.
//

#ifndef TASK2_CELLS_H
#define TASK2_CELLS_H
#include "vector"
#include <iostream>

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
    Point(int x, int y);
};

class GameObject {
protected:
    CellType object_type;
    GameObject();
public:
    CellType get_type();
};

class Cell{
public:
    std::vector<GameObject> objects;
};

class Mole: public GameObject{
private:
    static int get_random_number(int min, int max);
public:
    bool is_underground = false;
    bool is_mated = false;
    int default_steps_until_hide;
    int steps_until_hide;
    bool sex;

    static Point want_move_to(int field_width, int field_height);
    void try_get_underground();

    Mole(bool sex, int default_steps_until_hide);
};

class Cottager: public GameObject{
public:
    int move_speed;
    int hit_radius;

    Cottager(int move_speed, int hit_radius);
};

class Crop: public GameObject{
    const CellType cell_type = CellType::CROP;
};

#endif //TASK2_CELLS_H
