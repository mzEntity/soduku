#include "soduku.h"
#include "debug_utils/debug_utils.h"
#include "house.h"

const int Cell::POSSIBLE_COUNT = 9;

Cell::Cell() {
    this->candidates = std::vector<bool>(this->POSSIBLE_COUNT, true);
    this->value = -1;
    this->filled = false;
    this->fixed = false;

    this->row_belong = nullptr;
    this->col_belong = nullptr;
    this->box_belong = nullptr;
}

void Cell::fix(int num) {
    ASSERT(num >= 1 && num <= this->POSSIBLE_COUNT,
           "Number should be between 1 and 9.");
    ASSERT(!this->filled, "the cell is already filled.");
    this->value = num;
    this->filled = true;
    this->fixed = true;

    this->_remove_all_candidates_except(this->value);
}

void Cell::fill(int num) {
    ASSERT(num >= 1 && num <= this->POSSIBLE_COUNT,
           "Number should be between 1 and 9.");
    ASSERT(!this->filled, "the cell is already filled.");
    this->value = num;
    this->filled = true;

    this->_remove_all_candidates_except(this->value);
}

bool Cell::have_candidate(int num) {
    ASSERT(num >= 1 && num <= this->POSSIBLE_COUNT,
           "Number should be between 1 and 9.");
    int index = num - 1;

    return this->candidates[index];
}

int Cell::candidate_count() {
    int count = 0;
    for(int i = 1; i <= 9; i++) {
        if(this->have_candidate(i)) count += 1;
    }
    return count;
}

void Cell::remove_candidate(int num) {
    ASSERT(!this->filled, "the cell is already filled.");
    ASSERT(num >= 1 && num <= this->POSSIBLE_COUNT,
           "candidate should be between 1 and 9.");
    int index = num - 1;
    ASSERT(this->candidates[index], "remove a non-existent candidate.");

    this->candidates[index] = false;
}

std::vector<int> Cell::get_all_candidates(){
    std::vector<int> result;
    for(int i = 1; i <= 9; i++) {
        if(this->have_candidate(i)) result.push_back(i);
    }
    return result;
}

void Cell::_remove_all_candidates() {
    for (int i = 0; i < this->POSSIBLE_COUNT; i++) {
        this->candidates[i] = false;
    }
}

void Cell::_remove_all_candidates_except(int value) {
    ASSERT(this->have_candidate(value), "does't have this candidate.");
    for (int i = 0; i < this->POSSIBLE_COUNT; i++) {
        if (i == value - 1)
            continue;
        this->candidates[i] = false;
    }
}

const int Soduku::ROW_COUNT = 9;
const int Soduku::COL_COUNT = 9;

Soduku::Soduku() {
    LOG_INFO("soduku initializing...");
    for(int i = 0; i < this->ROW_COUNT; i++) {
        std::vector<Cell*> cell_row = std::vector<Cell*>();
        for(int j = 0; j < this->COL_COUNT; j++) {
            cell_row.push_back(new Cell());
        } 
        this->cells.push_back(cell_row);
    }

    std::vector<std::vector<Cell*>> rows = std::vector<std::vector<Cell*>>(
        this->ROW_COUNT, std::vector<Cell*>(this->COL_COUNT, nullptr));
    std::vector<std::vector<Cell*>> columns = std::vector<std::vector<Cell*>>(
        this->COL_COUNT, std::vector<Cell*>(this->ROW_COUNT, nullptr));

    // shape: (3, 3, 3, 3)
    std::vector<std::vector<std::vector<std::vector<Cell*>>>> boxes =
        std::vector<std::vector<std::vector<std::vector<Cell*>>>>(
            3, std::vector<std::vector<std::vector<Cell*>>>(
                   3, std::vector<std::vector<Cell*>>(
                          3, std::vector<Cell*>(3, nullptr))));

    for (int i = 0; i < this->ROW_COUNT; i++) {
        for (int j = 0; j < this->COL_COUNT; j++) {
            Cell* c = this->cells[i][j];
            rows[i][j] = c;
            columns[j][i] = c;
            boxes[i / 3][j / 3][i % 3][j % 3] = c;
        }
    }
    for (int i = 0; i < this->ROW_COUNT; i++) {
        this->rows.push_back(new Row(i + 1, rows[i]));
    }

    for (int i = 0; i < this->COL_COUNT; i++) {
        this->columns.push_back(new Column(i + 1, columns[i]));
    }

    for (int i = 0; i < 3; i++) {
        std::vector<Box*> box_row;
        for (int j = 0; j < 3; j++) {
            box_row.push_back(new Box(i * 3 + j + 1, boxes[i][j]));
        }
        this->boxes.push_back(box_row);
    }

    LOG_INFO("soduku initialization finished.");
}

void Soduku::init(std::vector<std::vector<int>> quest) {
    int row_count = quest.size();
    ASSERT(row_count == this->ROW_COUNT, "shape of the soduku doesn't match.");

    int col_count = quest[0].size();
    ASSERT(col_count == this->COL_COUNT, "shape of the soduku doesn't match.");

    for (int i = 0; i < row_count; i++) {
        for (int j = 0; j < col_count; j++) {
            if (quest[i][j] == -1)
                continue;
            this->cells[i][j]->fix(quest[i][j]);
        }
    }
}

void Soduku::fill(int row_num, int col_num, int value) {
    ASSERT(row_num >= 1 && row_num <= this->ROW_COUNT,
           "Row number should be between 1 and 9.");
    ASSERT(col_num >= 1 && col_num <= this->COL_COUNT,
           "Col number should be between 1 and 9.");
    this->cells[row_num - 1][col_num - 1]->fill(value);
}

void Soduku::remove_candidate(int row_num, int col_num, int value) {
    ASSERT(row_num >= 1 && row_num <= this->ROW_COUNT,
           "Row number should be between 1 and 9.");
    ASSERT(col_num >= 1 && col_num <= this->COL_COUNT,
           "Col number should be between 1 and 9.");
    this->cells[row_num - 1][col_num - 1]->remove_candidate(value);
}

Cell* Soduku::get_cell(int row_num, int col_num){
    ASSERT(row_num >= 1 && row_num <= this->ROW_COUNT,
           "Row number should be between 1 and 9.");
    ASSERT(col_num >= 1 && col_num <= this->COL_COUNT,
           "Col number should be between 1 and 9.");

    return this->cells[row_num - 1][col_num - 1];
}

void Soduku::print(std::ostream& out) {
    for (int i = 0; i < this->ROW_COUNT; i++) {
        if (i % 3 == 0) {
            for (int j = 0; j < this->COL_COUNT; j++) {
                out << "+=======";
            }
        } else {
            for (int j = 0; j < this->COL_COUNT; j++) {
                out << "+-------";
            }
        }

        out << "+" << std::endl;
        for (int j = 0; j < 3; j++) {
            for (int k = 0; k < this->COL_COUNT; k++) {
                std::string col_spliter = "|";
                if (k % 3 == 0)
                    col_spliter = "\u2551";
                out << col_spliter;
                for (int l = 0; l < 3; l++) {
                    int candidate = 1 + j * 3 + l;
                    out << " ";
                    if (this->cells[i][k]->have_candidate(candidate)) {
                        if (this->cells[i][k]->fixed) {
                            out << "\033[31m";
                        } else if (this->cells[i][k]->filled) {
                            out << "\033[36m";
                        }
                        out << candidate << "\033[0m";
                    } else {
                        out << " ";
                    }
                }
                out << " ";
            }
            out << "\u2551" << std::endl;
        }
    }
    for (int i = 0; i < this->COL_COUNT; i++) {
        out << "+=======";
    }
    out << "+" << std::endl;
}