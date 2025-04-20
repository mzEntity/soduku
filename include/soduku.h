#pragma once

#include <vector>
#include <iostream>

#include "house.h"

class Cell {
    static const int POSSIBLE_COUNT;

   public:
    std::vector<bool> candidates;
    int value;
    bool filled;
    bool fixed;
    Row* row_belong;
    Column* col_belong;
    Box* box_belong;

    Cell();

    void fix(int value);
    void fill(int value);

    bool have_candidate(int value);
    void remove_candidate(int num);

   private:
    void _remove_all_candidates();
    void _remove_all_candidates_except(int value);
};



class Soduku {
    static const int ROW_COUNT;
    static const int COL_COUNT;

   public:
    std::vector<std::vector<Cell*>> cells;
    std::vector<Row*> rows;
    std::vector<Column*> columns;
    std::vector<std::vector<Box*>> boxes;

    Soduku();

    void init(std::vector<std::vector<int>> quest);
    void fill(int row_num, int col_num, int value);
    void remove_candidate(int row_num, int col_num, int value);
    Cell* get_cell(int row_num, int col_num);
    
    void print(std::ostream& out);
};
