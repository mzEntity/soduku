#include "debug_utils/debug_utils.h"

#include "sudoku.h"
#include "solve/solutionlog.h"
#include "solve/link.h"
#include "solve/single_digit_patterns/turbot_fish.h"

#include <vector>

TurbotFishSolver::TurbotFishSolver(Sudoku* target, LinkManager* linkManager) {
    this->target = target;
    this->linkManager = linkManager;
}


bool TurbotFishSolver::solve() {
    using namespace std;
    for(int i = 0; i < 9; i++) {
        for(int j = 0; j < 9; j++) {
            vector<LinkNode*> nodes = this->linkManager->get_nodes(i+1, j+1);
            for(const auto& node: nodes) {
                bool change = this->_solve_node(node);
                if(change) return true;
            }
        }
    }
    return false;
}

bool TurbotFishSolver::_solve_node(LinkNode* node) {
    using namespace std;
    int value = node->candidate;
    Cell* cell = node->cell;
    vector<LinkNode*> strong_link_node_list1 = node->get_node_strong_linked_between();
    for(const auto& strong_link_node1: strong_link_node_list1) {
        vector<LinkNode*> weak_link_node_list1 = strong_link_node1->get_node_linked_between();
        for(const auto& weak_link_node1: weak_link_node_list1) {
            if(weak_link_node1 == node) continue;
            vector<LinkNode*> strong_link_node_list2 = weak_link_node1->get_node_strong_linked_between();
            for(const auto& strong_link_node2: strong_link_node_list2) {
                ASSERT(strong_link_node2 != node, "strong-weak-strong error");
                ASSERT(strong_link_node2->cell != cell, "strong-weak-strong error");
                vector<Cell*> cells_pair = {cell, strong_link_node2->cell};
                vector<Cell*> cells_intersect = this->target->get_cells_meet_all(cells_pair);

                vector<Cell*> cells_changed;
                for(Cell* target: cells_intersect) {
                    if(target->filled) continue;
                    if(!target->have_candidate(value)) continue;
                    target->remove_candidate(value);
                    cells_changed.push_back(target);
                }
                if(cells_changed.size() == 0) continue;
                string s = "TurbotFish: " + std::to_string(value) + " in " + cell->get_position() + "," + strong_link_node2->cell->get_position();
                s += "(connected by " + strong_link_node1->cell->get_position() + "," + weak_link_node1->cell->get_position() + ") => ";
                for(const auto& cell_changed: cells_changed) {
                    s += cell_changed->get_position() + ",";
                }
                s.pop_back();
                s += "<>" + std::to_string(value);
                SOLUTION_INFO(s);
                return true;
            }
        }
    }
    return false;
}