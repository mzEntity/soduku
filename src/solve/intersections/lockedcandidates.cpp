#include "solve/intersections/lockedcandidates.h"
#include "house.h"
#include "soduku.h"
#include "solve/solutionlog.h"

LockedCandidatesPointing::LockedCandidatesPointing(Soduku* target) {
    this->target = target;
}

bool LockedCandidatesPointing::solve() {
    for(const auto& box_row: this->target->boxes) {
        for(const auto& box: box_row) {
            bool change = this->_solve_box(box);
            if(change) return true;
        }
    }
    return false;
}

bool LockedCandidatesPointing::_solve_box(Box* b) {
    using namespace std;
    // int box_num = b->num;
    vector<vector<int>> candidate_row(9, vector<int>());
    vector<vector<int>> candidate_col(9, vector<int>());

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            Cell* target_cell = b->cells[i][j];
            if (target_cell->filled) {
                continue;
            }
            vector<int> contain_candidate = target_cell->get_all_candidates();
            for (const auto& candidate : contain_candidate) {
                candidate_row[candidate - 1].push_back(i);
                candidate_col[candidate - 1].push_back(j);
            }
        }
    }

    for (int i = 0; i < 9; i++) {
        int value = i + 1;
        vector<int> cur_candidate_rows = candidate_row[i];
        if (cur_candidate_rows.size() == 0)
            continue;
        int last_row = cur_candidate_rows[0];
        bool failed = false;
        for (const auto& r : cur_candidate_rows) {
            if (r != last_row) {
                failed = true;
                break;
            }
        }
        if (failed) continue;
        Row* cur_row = this->target->rows[last_row + ((b->num - 1) / 3) * 3];
        bool change = false;
        for(const auto& cell: cur_row->cells) {
            if(cell->box_belong == b) continue;
            if(!cell->have_candidate(value)) continue;
            cell->remove_candidate(value);
            change = true;
        }
        if(change) {
            std::cout << b->num << " " << cur_candidate_rows.size() << std::endl;
            SOLUTION_INFO("LockedCandidates Type 1(Pointing): r" + std::to_string(cur_row->num) + "=> " + std::to_string(value));
            return true;
        }
    }

    for (int i = 0; i < 9; i++) {
        int value = i + 1;
        vector<int> cur_candidate_cols = candidate_col[i];
        if (cur_candidate_cols.size() == 0)
            continue;
        int last_col = cur_candidate_cols[0];
        bool failed = false;
        for (const auto& c : cur_candidate_cols) {
            if (c != last_col) {
                failed = true;
                break;
            }
        }
        if (failed) continue;
        Column* cur_col = this->target->columns[last_col + ((b->num - 1) % 3) * 3];
        bool change = false;
        for(const auto& cell: cur_col->cells) {
            if(cell->box_belong == b) continue;
            if(!cell->have_candidate(value)) continue;
            cell->remove_candidate(value);
            change = true;
        }
        if(change) {
            SOLUTION_INFO("LockedCandidates Type 1(Pointing): c" + std::to_string(cur_col->num) + "=> " + std::to_string(value));
            return true;
        }
    }
    return false;
}


LockedCandidatesClaiming::LockedCandidatesClaiming(Soduku* target) {
    this->target = target;
}

bool LockedCandidatesClaiming::solve() {
    for(const auto& row: this->target->rows) {
        bool change = this->_solve_row(row);
        if(change) return true;
    }
    
    for(const auto& column: this->target->columns) {
        bool change = this->_solve_column(column);
        if(change) return true;
    }
    return false;
}

bool LockedCandidatesClaiming::_solve_row(Row* row) {
    using namespace std;
    vector<vector<int>> candidate_col(9, vector<int>());
    for(int i = 0; i < 9; i++) {
        Cell* c = row->cells[i];
        if(c->filled) continue;
        vector<int> candidates = c->get_all_candidates();
        for(const auto& candidate: candidates) {
            candidate_col[candidate-1].push_back(i);
        }
    }

    for(int i = 0; i < 9; i++) {
        int value = i + 1;
        vector<int> cur_candidate_cols = candidate_col[i];
        if(cur_candidate_cols.size()==0) continue;
        int last_box = cur_candidate_cols[0] / 3;
        bool failed = false;
        for(const auto& col: cur_candidate_cols) {
            if(col / 3 != last_box) {
                failed = true;
                break;
            }
        }
        if(failed) continue;
        Box* cur_box = this->target->boxes[(row->num - 1) / 3][last_box];
        bool change = false;
        for(const auto& cell_row: cur_box->cells) {
            for(const auto& cell: cell_row) {
                if(cell->row_belong == row) continue;
                if(!cell->have_candidate(value)) continue;
                cell->remove_candidate(value);
                change = true;
            }
        }
        if(change) {
            SOLUTION_INFO("LockedCandidates Type 2(Claiming): r" + std::to_string(row->num) + "=> " + std::to_string(value));
            return true;
        }
    }
    return false;
}


bool LockedCandidatesClaiming::_solve_column(Column* column) {
    using namespace std;
    vector<vector<int>> candidate_row(9, vector<int>());
    for(int i = 0; i < 9; i++) {
        Cell* c = column->cells[i];
        if(c->filled) continue;
        vector<int> candidates = c->get_all_candidates();
        for(const auto& candidate: candidates) {
            candidate_row[candidate-1].push_back(i);
        }
    }

    for(int i = 0; i < 9; i++) {
        int value = i + 1;
        vector<int> cur_candidate_rows = candidate_row[i];
        if(cur_candidate_rows.size()==0) continue;
        int last_box = cur_candidate_rows[0] / 3;
        bool failed = false;
        for(const auto& row: cur_candidate_rows) {
            if(row / 3 != last_box) {
                failed = true;
                break;
            }
        }
        if(failed) continue;
        Box* cur_box = this->target->boxes[last_box][(column->num - 1) / 3];
        bool change = false;
        for(const auto& cell_row: cur_box->cells) {
            for(const auto& cell: cell_row) {
                if(cell->col_belong == column) continue;
                if(!cell->have_candidate(value)) continue;
                cell->remove_candidate(value);
                change = true;
            }
        }
        if(change) {
            SOLUTION_INFO("LockedCandidates Type 2(Claiming): c" + std::to_string(column->num) + "=> " + std::to_string(value));
            return true;
        }
    }
    return false;
}