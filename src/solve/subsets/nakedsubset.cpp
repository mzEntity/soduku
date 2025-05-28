#include "solve/subsets/nakedsubset.h"
#include "house.h"
#include "sudoku.h"
#include "solve/solutionlog.h"

#include <queue>
#include <set>
#include <utility>
#include <vector>

NakedSubsetSolver::NakedSubsetSolver(Sudoku* target) {
    this->target = target;
}

bool NakedSubsetSolver::solve() {
    for (const auto& row : this->target->rows) {
        bool change = this->_solve_row(row);
        if (change)
            return true;
    }

    for (const auto& column : this->target->columns) {
        bool change = this->_solve_column(column);
        if (change)
            return true;
    }

    for (const auto& box_row : this->target->boxes) {
        for (const auto& box : box_row) {
            bool change = this->_solve_box(box);
            if (change)
                return true;
        }
    }
    return false;
}

std::pair<std::set<Cell*>, std::set<int>> NakedSubsetSolver::_naked_subset(std::vector<Cell*> cells,
                                                       int length) {
    using namespace std;
    queue<vector<int>> subset_values_list;

    for (int i = 0; i < 9; i++) {
        subset_values_list.push(vector<int>(1, i + 1));
    }
    while (!subset_values_list.empty()) {
        vector<int> top = subset_values_list.front();
        if (top.size() == length)
            break;
        subset_values_list.pop();
        int max_value = top[top.size() - 1];
        for (int i = max_value + 1; i <= 9; i++) {
            vector<int> temp_subset(top);
            temp_subset.push_back(i);
            subset_values_list.push(temp_subset);
        }
    }

    while (!subset_values_list.empty()) {
        set<int> subset_values(subset_values_list.front().begin(),
                               subset_values_list.front().end());
        subset_values_list.pop();

        set<Cell*> result;
        for (const auto& cell : cells) {
            if (cell->filled)
                continue;
            vector<int> candidates = cell->get_all_candidates();
            if (candidates.size() > length)
                continue;
            bool contain_other_value = false;
            for (const auto& candidate : candidates) {
                if (!subset_values.contains(candidate)) {
                    contain_other_value = true;
                    break;
                }
            }
            if (contain_other_value)
                continue;
            result.insert(cell);
        }

        if (result.size() == length) {
            return make_pair(result, subset_values);
        }
    }

    return make_pair(set<Cell*>(), set<int>());
}

bool NakedSubsetSolver::_work(std::set<Cell*> subset, std::set<int> values) {
    using namespace std;

    Row* row = nullptr;
    Column* column = nullptr;
    Box* box = nullptr;
    bool same_row = true;
    bool same_column = true;
    bool same_box = true;

    for (const auto& cell : subset) {
        if (row == nullptr) {
            row = cell->row_belong;
        } else if (row != cell->row_belong) {
            same_row = false;
        }

        if (column == nullptr) {
            column = cell->col_belong;
        } else if (column != cell->col_belong) {
            same_column = false;
        }

        if (box == nullptr) {
            box = cell->box_belong;
        } else if (box != cell->box_belong) {
            same_box = false;
        }
    }
    bool change = false;

    if (same_row) {
        vector<Cell*> cells = row->cells;
        for (const auto& cell : cells) {
            if (cell->filled)
                continue;
            if (subset.contains(cell))
                continue;
            for (const auto& value : values) {
                if (cell->have_candidate(value)) {
                    cell->remove_candidate(value);
                    change = true;
                }
            }
        }
    }

    if (same_column) {
        vector<Cell*> cells = column->cells;
        for (const auto& cell : cells) {
            if (cell->filled)
                continue;
            if (subset.contains(cell))
                continue;
            for (const auto& value : values) {
                if (cell->have_candidate(value)) {
                    cell->remove_candidate(value);
                    change = true;
                }
            }
        }
    }

    if (same_box) {
        for (const auto& cell_row : box->cells) {
            for (const auto& cell : cell_row) {
                if (cell->filled)
                    continue;
                if (subset.contains(cell))
                    continue;
                for (const auto& value : values) {
                    if (cell->have_candidate(value)) {
                        cell->remove_candidate(value);
                        change = true;
                    }
                }
            }
        }
    }

    return change;
}

bool NakedSubsetSolver::_solve_row(Row* row) {
    using namespace std;
    vector<Cell*> cells = row->cells;
    for (int i = 1; i <= 4; i++) {
        pair<set<Cell*>, set<int>> result = this->_naked_subset(cells, i);
        set<Cell*> subset = result.first;
        if (subset.size() == 0)
            continue;
        set<int> values = result.second;
        bool change = this->_work(subset, values);

        if (change) {
            string s = "NakedSubset: ";
            for (const auto& value : values) {
                s.append(to_string(value) + ",");
            }
            s.pop_back();
            s.append(" in ");
            for (const auto& cell : subset) {
                s.append(cell->get_position() + ",");
            }
            s.pop_back();
            SOLUTION_INFO(s);
            return true;
        }
    }
    return false;
}

bool NakedSubsetSolver::_solve_column(Column* column) {
    using namespace std;
    vector<Cell*> cells = column->cells;
    for (int i = 1; i <= 4; i++) {
        pair<set<Cell*>, set<int>> result = this->_naked_subset(cells, i);
        set<Cell*> subset = result.first;
        if (subset.size() == 0)
            continue;
        set<int> values = result.second;
        bool change = this->_work(subset, values);

        if (change) {
            string s = "NakedSubset: ";
            for (const auto& value : values) {
                s.append(to_string(value) + ",");
            }
            s.pop_back();
            s.append(" in ");
            for (const auto& cell : subset) {
                s.append(cell->get_position() + ",");
            }
            s.pop_back();
            SOLUTION_INFO(s);
            return true;
        }
    }
    return false;
}

bool NakedSubsetSolver::_solve_box(Box* box) {
    using namespace std;
    vector<Cell*> cells;
    for (const auto& cell_row : box->cells) {
        for (const auto& cell : cell_row) {
            cells.push_back(cell);
        }
    }
    for (int i = 1; i <= 4; i++) {
        pair<set<Cell*>, set<int>> result = this->_naked_subset(cells, i);
        set<Cell*> subset = result.first;
        if (subset.size() == 0)
            continue;
        set<int> values = result.second;
        bool change = this->_work(subset, values);

        if (change) {
            string s = "NakedSubset: ";
            for (const auto& value : values) {
                s.append(to_string(value) + ",");
            }
            s.pop_back();
            s.append(" in ");
            for (const auto& cell : subset) {
                s.append(cell->get_position() + ",");
            }
            s.pop_back();
            SOLUTION_INFO(s);
            return true;
        }
    }
    return false;
}
