#pragma once

#include <vector>
#include <iostream>

#include "house.h"

class Cell {
    static const int POSSIBLE_COUNT;

   public:
    std::vector<bool> candidates;   // 每一个格子可以填写的候选（true表示可填，false表示不可填）
    int value;  // 仅当 filled 为true时有效，表示填入的数字
    bool filled;    // 该格子是否已经填入数字，如果 fixed 为true，则 filled 必定为true
    bool fixed; // 该格子是否为题目固定的数字
    Row* row_belong;    // 格子所在的行
    Column* col_belong; // 格子所在的列
    Box* box_belong;    // 格子所在的宫

    Cell();

    void fix(int value);
    void fill(int value);

    bool have_candidate(int value); // 是否包含某一候选，要求 filled 为false
    int candidate_count();  // 获取候选个数
    void remove_candidate(int num); // 移除某一候选，要求包含该候选，num为1-9

    std::vector<int> get_all_candidates();

    std::string get_position();

    bool is_peer(Cell* other);

   private:
    void _remove_all_candidates();
    void _remove_all_candidates_except(int value);
};



class Sudoku {
    static const int ROW_COUNT;
    static const int COL_COUNT;

   public:
    std::vector<std::vector<Cell*>> cells;
    std::vector<Row*> rows;
    std::vector<Column*> columns;
    std::vector<std::vector<Box*>> boxes;

    Sudoku();

    void init(std::vector<std::vector<int>> quest);
    void fill(int row_num, int col_num, int value);
    void remove_candidate(int row_num, int col_num, int value);
    Cell* get_cell(int row_num, int col_num);

    std::vector<Cell*> get_cells_meet_all(std::vector<Cell*> cells);
    
    void print(std::ostream& out);
};
