#pragma once

#include <vector>

class Cell;

class House {
   public:
    virtual void remove_candidate_from(Cell* c) = 0;
};

class Row : public House {
    static const int CELL_COUNT;

   public:
    int num;
    std::vector<Cell*> cells;
    Row(int row_num, std::vector<Cell*> cells);

    void remove_candidate_from(Cell* c) override;
};

class Column : public House {
    static const int CELL_COUNT;

   public:
    int num;
    std::vector<Cell*> cells;
    Column(int col_num, std::vector<Cell*> cells);

    void remove_candidate_from(Cell* c) override;
};

class Box : public House {
    static const int ROW_COUNT;
    static const int COL_COUNT;

   public:
    int num;
    std::vector<std::vector<Cell*>> cells;
    Box(int num, std::vector<std::vector<Cell*>> cells);

    void remove_candidate_from(Cell* c) override;
};