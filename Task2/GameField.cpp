//
// Created by Harmony on 16.03.2023.
//

#include "GameField.h"

GameField::GameField(int x_size, int y_size, Cell **field) : x_size(x_size), y_size(y_size), field(field) {

}

Cell GameField::get(int y, int x) {
    return field[y][x];
}

bool GameField::is_inside_field(int y, int x) const {
    return x > 0 && x < x_size && y > 0 && y < y_size;
}

void GameField::set(int y, int x, Cell cell) {
    field[y][x] = cell;
}

GameField::GameField() {

}

void GameField::print_field(std::map<Point, Mole> mole_map) {
    std::cout << "==================================================" << std::endl;
    for (int y = 0; y < y_size; y++){
        for (int x = 0; x < x_size; x++){
            Cell cell = field[y][x];
            if (cell.cell_type == CellType::EMPTY){
                std::cout << "o";
            } else if (cell.cell_type == CellType::CROP){
                std::cout << "@";
            } else if (cell.cell_type == CellType::MOLE){
                if (mole_map[Point(y, x)].is_underground) {
                    std::cout << "*";
                }
                else {
                    std::cout << "%";
                }
            } else if (cell.cell_type == CellType::COTTAGER){
                std::cout << "C";
            }
        }
        std::cout << std::endl;
    }
    std::cout << "==================================================" << std::endl;
}
