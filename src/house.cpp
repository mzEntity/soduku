#include "house.h"
#include "debug_utils/debug_utils.h"
#include "soduku.h"

const int Row::CELL_COUNT = 9;
const int Column::CELL_COUNT = 9;
const int Box::ROW_COUNT = 3;
const int Box::COL_COUNT = 3;

Row::Row(int row_num, std::vector<Cell*> cells) {
    ASSERT(row_num >= 1 && row_num <= 9, "row num should be between 1 and 9.");
    ASSERT(cells.size() == this->CELL_COUNT, "cell count in one row is 9.");
    this->num = row_num;
    this->cells = std::vector<Cell*>();
    for (int i = 0; i < this->CELL_COUNT; i++) {
        ASSERT(cells[i] != nullptr, "cell is nullptr");
        this->cells.push_back(cells[i]);
    }

    for (int i = 0; i < this->CELL_COUNT; i++) {
        this->cells[i]->row_belong = this;
    }
}

void Row::remove_candidate_from(Cell* c) {
    ASSERT(c->filled, "cell hasn't been filled.");
    int value = c->value;
    for (int i = 0; i < this->CELL_COUNT; i++) {
        Cell* target = this->cells[i];
        if (target == c || !target->have_candidate(value))
            continue;
        target->remove_candidate(value);
    }
}

Column::Column(int col_num, std::vector<Cell*> cells) {
    ASSERT(col_num >= 1 && col_num <= 9, "col num should be between 1 and 9.");
    ASSERT(cells.size() == this->CELL_COUNT, "cell count in one row is 9.");
    this->num = col_num;
    this->cells = std::vector<Cell*>();
    for (int i = 0; i < this->CELL_COUNT; i++) {
        ASSERT(cells[i] != nullptr, "cell is nullptr");
        this->cells.push_back(cells[i]);
    }

    for (int i = 0; i < this->CELL_COUNT; i++) {
        this->cells[i]->col_belong = this;
    }
}

void Column::remove_candidate_from(Cell* c) {
    ASSERT(c->filled, "cell hasn't been filled.");
    int value = c->value;
    for (int i = 0; i < this->CELL_COUNT; i++) {
        Cell* target = this->cells[i];
        if (target == c || !target->have_candidate(value))
            continue;
        target->remove_candidate(value);
    }
}

Box::Box(int box_num, std::vector<std::vector<Cell*>> cells) {
    ASSERT(box_num >= 1 && box_num <= 9, "row num should be between 1 and 9.");
    ASSERT(cells.size() == this->ROW_COUNT, "In one box, there is 3 rows.");
    ASSERT(cells[0].size() == this->COL_COUNT,
           "In one box, there is 3 columns.");
    this->num = box_num;
    this->cells = std::vector<std::vector<Cell*>>();
    for (int i = 0; i < this->ROW_COUNT; i++) {
        std::vector<Cell*> row = std::vector<Cell*>();
        for (int j = 0; j < this->COL_COUNT; j++) {
            ASSERT(cells[i][j] != nullptr, "cell is nullptr");
            row.push_back(cells[i][j]);
        }
        this->cells.push_back(row);
    }

    for (int i = 0; i < this->ROW_COUNT; i++) {
        for (int j = 0; j < this->COL_COUNT; j++) {
            this->cells[i][j]->box_belong = this;
        }
    }
}

void Box::remove_candidate_from(Cell* c) {
    ASSERT(c->filled, "cell hasn't been filled.");
    int value = c->value;
    for (int i = 0; i < this->ROW_COUNT; i++) {
        for (int j = 0; j < this->COL_COUNT; j++) {
            Cell* target = this->cells[i][j];
            if (target == c || !target->have_candidate(value))
                continue;
            target->remove_candidate(value);
        }
    }
}