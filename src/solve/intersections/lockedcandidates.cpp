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