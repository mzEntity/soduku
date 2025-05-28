#include "solve/subsets/hiddensubset.h"
#include "sudoku.h"
#include "solve/solutionlog.h"

#include <queue>
#include <set>
#include <utility>
#include <vector>

HiddenSubsetSolver::HiddenSubsetSolver(Sudoku* target) {
    this->target = target;
}

bool HiddenSubsetSolver::solve() {
    for (int i = 1; i <= 4; i++) {
        for (const auto& row : this->target->rows) {
            bool change = this->_solve_row(row, i);
            if (change)
                return true;
        }

        for (const auto& column : this->target->columns) {
            bool change = this->_solve_column(column, i);
            if (change)
                return true;
        }

        for (const auto& box_row : this->target->boxes) {
            for (const auto& box : box_row) {
                bool change = this->_solve_box(box, i);
                if (change)
                    return true;
            }
        }
    }
    return false;
}

bool HiddenSubsetSolver::_work(std::set<Cell*> subset, std::set<int> values) {
    using namespace std;
    bool change = false;
    for (const auto& cell : subset) {
        vector<int> candidates = cell->get_all_candidates();
        for (const auto& candidate : candidates) {
            if (values.contains(candidate))
                continue;
            cell->remove_candidate(candidate);
            change = true;
        }
    }
    return change;
}

std::pair<std::set<Cell*>, std::set<int>> HiddenSubsetSolver::_hidden_subset(
    std::vector<Cell*> cells,
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
        bool failed = false;
        for (const auto& cell : cells) {
            if (cell->filled) {
                if(subset_values.contains(cell->value)) {
                    failed = true;
                    break;
                }
                continue;
            }
                
            bool contain_value = false;
            for (const auto& value : subset_values) {
                if (cell->have_candidate(value)) {
                    contain_value = true;
                    break;
                }
            }
            if (!contain_value)
                continue;
            result.insert(cell);
        }

        if(failed) continue;

        if (result.size() == length) {
            return make_pair(result, subset_values);
        }
    }

    return make_pair(set<Cell*>(), set<int>());
}

bool HiddenSubsetSolver::_solve_row(Row* row, int length) {
    using namespace std;
    vector<Cell*> cells = row->cells;
    pair<set<Cell*>, set<int>> result = this->_hidden_subset(cells, length);
    set<Cell*> subset = result.first;
    if (subset.size() == 0)
        return false;
    set<int> values = result.second;

    bool change = this->_work(subset, values);
    if (!change)
        return false;

    string s = "HiddenSubset: ";
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

bool HiddenSubsetSolver::_solve_column(Column* column, int length) {
    using namespace std;
    vector<Cell*> cells = column->cells;
    pair<set<Cell*>, set<int>> result = this->_hidden_subset(cells, length);
    set<Cell*> subset = result.first;
    if (subset.size() == 0)
        return false;
    set<int> values = result.second;

    bool change = this->_work(subset, values);
    if (!change)
        return false;

    string s = "HiddenSubset: ";
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

bool HiddenSubsetSolver::_solve_box(Box* box, int length) {
    using namespace std;
    vector<Cell*> cells;
    for (const auto& cell_row : box->cells) {
        for (const auto& cell : cell_row) {
            cells.push_back(cell);
        }
    }
    pair<set<Cell*>, set<int>> result = this->_hidden_subset(cells, length);
    set<Cell*> subset = result.first;
    if (subset.size() == 0)
        return false;
    set<int> values = result.second;

    bool change = this->_work(subset, values);
    if (!change)
        return false;

    string s = "HiddenSubset: ";
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
