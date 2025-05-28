#include "solve/singles/hiddensingle.h"
#include "debug_utils/debug_utils.h"
#include "solve/solutionlog.h"

#include "sudoku.h"

HiddenSingleSolver::HiddenSingleSolver(Sudoku* target) {
    this->target = target;
}

Cell* HiddenSingleSolver::solve() {
    Cell* c = this->_solve_row();
    if (c != nullptr)
        return c;
    c = this->_solve_column();
    if (c != nullptr)
        return c;
    c = this->_solve_box();
    return c;
}

void HiddenSingleSolver::_work(Cell* c, int value) {
    ASSERT(!c->filled, "cell has already been filled.");
    c->fill(value);
}

Cell* HiddenSingleSolver::_solve_row() {
    using namespace std;

    for (const auto& r : this->target->rows) {
        vector<int> candidate_count(9, 0);
        vector<Cell*> last_cell(9, nullptr);
        for (const auto& cell : r->cells) {
            vector<int> candidates = cell->get_all_candidates();
            for (const auto& candidate : candidates) {
                candidate_count[candidate - 1] += 1;
                last_cell[candidate - 1] = cell;
            }
        }

        for (int i = 0; i < 9; i++) {
            if (candidate_count[i] == 1) {
                Cell* target_c = last_cell[i];
                if (target_c->filled) {
                    continue;
                }
                this->_work(target_c, i + 1);
                SOLUTION_INFO("HiddenSingle: " + target_c->get_position() + "=" + std::to_string(i+1));
                return target_c;
            }
        }
    }

    return nullptr;
}

Cell* HiddenSingleSolver::_solve_column() {
    using namespace std;

    for (const auto& column : this->target->columns) {
        vector<int> candidate_count(9, 0);
        vector<Cell*> last_cell(9, nullptr);
        for (const auto& cell : column->cells) {
            vector<int> candidates = cell->get_all_candidates();
            for (const auto& candidate : candidates) {
                candidate_count[candidate - 1] += 1;
                last_cell[candidate - 1] = cell;
            }
        }

        for (int i = 0; i < 9; i++) {
            if (candidate_count[i] == 1) {
                Cell* target_c = last_cell[i];
                if (target_c->filled) {
                    continue;
                }
                this->_work(target_c, i + 1);
                SOLUTION_INFO("HiddenSingle: " + target_c->get_position() + "=" + std::to_string(i+1));
                return target_c;
            }
        }
    }

    return nullptr;
}

Cell* HiddenSingleSolver::_solve_box() {
    using namespace std;

    for (const auto& box_row : this->target->boxes) {
        for (const auto& box : box_row) {
            vector<int> candidate_count(9, 0);
            vector<Cell*> last_cell(9, nullptr);
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    Cell* cell = box->cells[i][j];
                    vector<int> candidates = cell->get_all_candidates();
                    for (const auto& candidate : candidates) {
                        candidate_count[candidate - 1] += 1;
                        last_cell[candidate - 1] = cell;
                    }
                }
            }

            for (int i = 0; i < 9; i++) {
                if (candidate_count[i] == 1) {
                    Cell* target_c = last_cell[i];
                    if (target_c->filled) {
                        continue;
                    }
                    this->_work(target_c, i + 1);
                    SOLUTION_INFO("HiddenSingle: " + target_c->get_position() + "=" + std::to_string(i+1));
                    return target_c;
                }
            }
        }
    }

    return nullptr;
}
