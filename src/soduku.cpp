#include "soduku.h"
#include "debug_utils/debug_utils.h"

const int Cell::POSSIBLE_COUNT = 9;

Cell::Cell() {
    this->candidates = std::vector<bool>(this->POSSIBLE_COUNT, true);
    this->value = -1;
    this->filled = false;
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

void Cell::remove_candidate(int num) {
    ASSERT(!this->filled, "the cell is already filled.");
    ASSERT(num >= 1 && num <= this->POSSIBLE_COUNT,
           "candidate should be between 1 and 9.");
    int index = num - 1;
    ASSERT(this->candidates[index], "remove a non-existent candidate.");

    this->candidates[index] = false;
}

void Cell::_remove_all_candidates() {
    for (int i = 0; i < this->POSSIBLE_COUNT; i++) {
        this->candidates[i] = false;
    }
}

void Cell::_remove_all_candidates_except(int value) {
    ASSERT(this->have_candidate(value), "does't have this candidate.");
    for (int i = 0; i < this->POSSIBLE_COUNT; i++) {
        if(i == value - 1) continue;
        this->candidates[i] = false;
    }
}

const int Soduku::ROW_COUNT = 9;
const int Soduku::COL_COUNT = 9;

Soduku::Soduku() {
    this->cells = std::vector<std::vector<Cell>>(
        this->ROW_COUNT, std::vector<Cell>(this->COL_COUNT, Cell()));
}

void Soduku::fill(std::vector<std::vector<int>> quest) {
    int row_count = quest.size();
    ASSERT(row_count == this->ROW_COUNT, "shape of the soduku doesn't match.");

    int col_count = quest[0].size();
    ASSERT(col_count == this->COL_COUNT, "shape of the soduku doesn't match.");

    for (int i = 0; i < row_count; i++) {
        for (int j = 0; j < col_count; j++) {
            if (quest[i][j] == -1)
                continue;
            this->cells[i][j].fill(quest[i][j]);
        }
    }
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
                    if (this->cells[i][k].have_candidate(candidate)) {
                        out << candidate;
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