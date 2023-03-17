//
// Created by Harmony on 15.03.2023.
//

#include "game.h"
#define DEFAULT_STEPS_UNTIL_HIDE 1
using namespace std;

Game::Game(int field_width, int field_height, int moles_count, int move_speed, int hit_radius) {
    Cell **field_raw = new Cell * [field_height];
    moles_count = moles_count;
    for (int i = 0; i < field_height; i++)
    {
        field_raw[i] = new Cell [field_width];
    }
    field_raw[0][0].cell_type = CellType::COTTAGER;
    cottager = Cottager(move_speed, hit_radius);
    cottager_position = Point(0, 0);
    field = GameField(field_width, field_height, field_raw);
    crop_num = 0;
    for (int i = 0; i < moles_count; i++){
        generate_mole();
    }
    generate_crop();
}

void Game::start() {
    while (!moles_map.empty() && crop_num > 0){
        field.print_field(moles_map);
        for (auto item = moles_map.begin(); item != moles_map.end(); ) {
            if (item->second.has_actioned){
                item->second.has_actioned = false;
                item++;
                continue;
            }
            cout << "Mole action at " << item->first << endl;
            mole_action(item->first, item->second);
            field.print_field(moles_map);
            item = moles_map.begin();
        }
        cout << "Cottager action at " << cottager_position << endl;
        Point nearest_mole = find_nearest_mole();
        if (nearest_mole.x == INT16_MAX){
            cout << "Cottager skipped" << endl;
            continue;
        }
        double distance_to_mole = sqrt(pow(nearest_mole.y - cottager_position.y, 2) +
                                       pow(nearest_mole.x - cottager_position.x, 2));
        if (distance_to_mole <= cottager.hit_radius){
            kill_mole(nearest_mole);
        } else {
            move_cottager_to(nearest_mole);
        }
        field.print_field(moles_map);
    }
    cout << "Game has ended. Crop num: " << crop_num << ". Moles count: " << moles_map.size();
}

void Game::mole_action(Point mole_coords, Mole& mole) {
    if (mole.is_underground){
         if (!mole.has_moved) {
            Point mole_want = mole.want_move_to(field.x_size, field.y_size);
            while (field.get(mole_want.y, mole_want.x).cell_type != CellType::EMPTY){
                mole_want = mole.want_move_to(field.x_size, field.y_size);
            }
            field.set(mole_coords.y, mole_coords.x, Cell());
            field.set(mole_want.y, mole_want.x, Cell(CellType::MOLE));
            mole.has_actioned = true;
            mole.has_moved = true;
            moles_map[mole_want] = mole;
            moles_map.erase(mole_coords);
            cout << "Mole has moved from " << mole_coords << " to " << mole_want << endl;
        } else if (!mole.is_mated && find_mate(mole_coords, mole)){
             mole.is_mated = true;
             mole.has_actioned = true;
         } else {
            mole.is_underground = false;
            cout << "Mole at " << mole_coords << " now is on ground." << endl;
            mole.has_actioned = true;
        }
    } else {
        if (mole.steps_until_hide == 1){
            harvest_crop(mole_coords);
        }
        cout << "Mole at " << mole_coords << " try to get underground." << endl;
        mole.try_get_underground();
        mole.has_actioned = true;
    }
}

bool Game::find_mate(Point mole_coords, Mole& mole) {
    if (mole.is_mated){
        return false;
    }
    Point potential_moles[4] =
            {Point(mole_coords.y, mole_coords.x - 1), Point(mole_coords.y, mole_coords.x + 1),
             Point(mole_coords.y, mole_coords.x), Point(mole_coords.y + 1, mole_coords.x)};

    for (Point potential_mole : potential_moles){
        if (check_type(potential_mole, CellType::MOLE)){
            Mole& second_mole = moles_map.at(potential_mole);
            if (second_mole.is_underground && !second_mole.is_mated && second_mole.sex - mole.sex != 0){
                second_mole.is_mated = true;
                generate_mole();
                cout << "Two moles " << potential_mole << " and " << mole_coords << " created new mole." << endl;
                return true;
            }
        }
    }
    return false;
}

bool Game::check_type(Point potential_mole_coords, CellType cell_type) {
    return field.is_inside_field(potential_mole_coords.y, potential_mole_coords.x) &&
           field.get(potential_mole_coords.y, potential_mole_coords.x).cell_type == cell_type;
}

void Game::generate_mole() {
    for (int i = 0; i < field.y_size; i++){
        for (int j = 0; j < field.x_size; j++){
            if (field.get(i, j).cell_type == CellType::EMPTY){
                Mole new_mole = Mole((bool) i % 2, DEFAULT_STEPS_UNTIL_HIDE);
                Cell mole_cell = Cell(CellType::MOLE);
                field.set(i, j, mole_cell);
                moles_map[Point(i, j)] = new_mole;
                return;
            }
        }
    }
}

void Game::generate_crop() {
    for (int i = 0; i < field.y_size; i++){
        for (int j = 0; j < field.x_size; j++){
            if (field.get(i, j).cell_type == CellType::EMPTY){
                Cell crop_cell = Cell(CellType::CROP);
                field.set(i, j, crop_cell);
                crop_num += 1;
                return;
            }
        }
    }
}

void Game::harvest_crop(Point mole_coords) {
    Point potential_crops[4] =
            {Point(mole_coords.y, mole_coords.x - 1), Point(mole_coords.y, mole_coords.x + 1),
             Point(mole_coords.y - 1, mole_coords.x), Point(mole_coords.y + 1, mole_coords.x)};
    for (Point point : potential_crops){
        if (check_type(point, CellType::CROP)){
            field.set(point.y, point.x, Cell());
            crop_num -= 1;
            cout << "Mole harvested crop at " << point << endl;
        }
    }
}

Point Game::find_nearest_mole() {
    Point nearest_point = Point(INT16_MAX, INT16_MAX);
    for (auto& item : moles_map){
        if (item.second.is_underground){
            continue;
        }
        Point mole_point = item.first;
        double distance_to_nearest = sqrt(pow(cottager_position.x - nearest_point.x, 2) +
                pow(cottager_position.y - nearest_point.y, 2));
        double distance_to_mole = sqrt(pow(cottager_position.x - mole_point.x, 2) +
                pow(cottager_position.y - mole_point.y, 2));
        if (distance_to_nearest > distance_to_mole){
            nearest_point = mole_point;
        }
    }
    return nearest_point;
}

void Game::move_cottager_to(Point move_to) {
    Point final_move;
    if (abs(cottager_position.y - move_to.y) > abs(cottager_position.x - move_to.x)){
        int distance_of_moving = abs(cottager_position.y - move_to.y) < cottager.move_speed ?
                                 abs(cottager_position.y - move_to.y) : cottager.move_speed;
        final_move.x = cottager_position.x;
        final_move.y = cottager_position.y - (cottager_position.y - move_to.y) / abs(cottager_position.y - move_to.y) * (distance_of_moving);
    } else if (abs(cottager_position.y - move_to.y) < abs(cottager_position.x - move_to.x)){
        int distance_of_moving = abs(cottager_position.x - move_to.x) < cottager.move_speed ?
                                 abs(cottager_position.x - move_to.x) : cottager.move_speed;
        final_move.y = cottager_position.y;
        final_move.x = cottager_position.x - (cottager_position.x - move_to.x) / abs(cottager_position.x - move_to.x) * (distance_of_moving);
    } else {
        int distance_of_moving = abs(cottager_position.y - move_to.y) < (int) cottager.move_speed / 2 ?
                                 abs(cottager_position.y - move_to.y) : (int) cottager.move_speed;
        final_move.y = cottager_position.y - (cottager_position.y - move_to.y) / abs(cottager_position.y - move_to.y) * (distance_of_moving);
        final_move.x = cottager_position.x - (cottager_position.x - move_to.x) / abs(cottager_position.x - move_to.x) * (distance_of_moving);
    }
    if (field.get(cottager_position.y, cottager_position.x).cell_type == CellType::CROP){
        crop_num -= 1;
    }
    if (field.get(final_move.y, final_move.x).cell_type != CellType::EMPTY){
        return;
    }
    field.set(cottager_position.y, cottager_position.x, Cell());
    cottager_position = final_move;
    field.set(cottager_position.y, cottager_position.x, Cell(CellType::COTTAGER));
    cout << "Moved cottager to: " << cottager_position << endl;
}

void Game::kill_mole(Point mole_pos) {
    moles_map.erase(mole_pos);
    field.set(mole_pos.y, mole_pos.x, Cell());
    cout << "Killed mole at: " << mole_pos << endl;
}


