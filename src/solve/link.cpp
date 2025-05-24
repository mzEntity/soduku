#include "soduku.h"
#include "house.h"
#include "solve/link.h"
#include <vector>
#include <map>

LinkNode::LinkNode(Cell* cell, int candidate) {
    this->cell = cell;
    this->candidate = candidate;
}

Link::Link(LinkNode* node1, LinkNode* node2, link_type type){
    this->node1 = node1;
    this->node2 = node2;
    this->type = type;
}

std::string Link::to_string() {
    int value = this->node1->candidate;
    return this->node1->cell->get_position() + "--" + std::to_string(value) + "--" + this->node2->cell->get_position();
}

LinkManager::LinkManager(Soduku* target) {
    this->target = target;
    for(int i = 0; i < 9; i++) {
        int value = i + 1;
        this->strong_link_between_list.insert({value, std::vector<Link*>()});
        this->weak_link_between_list.insert({value, std::vector<Link*>()});
    }
}

void LinkManager::build() {
    for(const auto& row: this->target->rows) {
        this->_build_row(row);
    }

    for(const auto& column: this->target->columns) {
        this->_build_column(column);
    }

    for(const auto& box_row: this->target->boxes) {
        for(const auto& box: box_row) {
            this->_build_box(box);
        }
    }
}

void LinkManager::print_all_links(){
    using namespace std;
    cout << "strong links: " << endl;
    for(int i = 0; i < 9; i++) {
        int value = i + 1;
        vector<Link*> strong_links = this->strong_link_between_list[value];
        if(strong_links.size() == 0) continue;
        cout << "\tvalue " << value << ": " << endl;
        for(const auto& link: strong_links) {
            cout << "\t\t" << link->to_string() << endl;
        }
    }

    cout << "weak links: " << endl;
    for(int i = 0; i < 9; i++) {
        int value = i + 1;
        vector<Link*> weak_links = this->weak_link_between_list[value];
        if(weak_links.size() == 0) continue;
        cout << "\tvalue " << value << ": " << endl;
        for(const auto& link: weak_links) {
            cout << "\t\t" << link->to_string() << endl;
        }
    }
}

void LinkManager::_build_row(Row* row){
    using namespace std;
    vector<Cell*> cells = row->cells;
    map<int, vector<Cell*>> cells_with_value;
    for(int i = 0; i < 9; i++) {
        cells_with_value.insert({i+1, vector<Cell*>()});
    }

    for(const auto& cell: cells) {
        if(cell->filled) continue;
        vector<int> candidates = cell->get_all_candidates();
        for(const auto& candidate: candidates){
            cells_with_value[candidate].push_back(cell);
        }
    }

    for(int i = 0; i < 9; i++) {
        int value = i + 1;
        vector<Cell*> cur_cells_with_value = cells_with_value[value];
        int cell_count = cur_cells_with_value.size();
        for(int j = 0; j < cell_count; j++) {
            for(int k = j + 1; k < cell_count; k++) {
                Link* link = nullptr;
                if(cell_count > 2) {
                    link = this->_build_weak_link(cur_cells_with_value[j], cur_cells_with_value[k], value);
                    this->weak_link_between_list[value].push_back(link);
                } else {
                    link = this->_build_strong_link(cur_cells_with_value[j], cur_cells_with_value[k], value);
                    this->strong_link_between_list[value].push_back(link);
                }
            }
        }
    }
}

void LinkManager::_build_column(Column* column){
    using namespace std;
    vector<Cell*> cells = column->cells;
    map<int, vector<Cell*>> cells_with_value;
    for(int i = 0; i < 9; i++) {
        cells_with_value.insert({i+1, vector<Cell*>()});
    }

    for(const auto& cell: cells) {
        if(cell->filled) continue;
        vector<int> candidates = cell->get_all_candidates();
        for(const auto& candidate: candidates){
            cells_with_value[candidate].push_back(cell);
        }
    }

    for(int i = 0; i < 9; i++) {
        int value = i + 1;
        vector<Cell*> cur_cells_with_value = cells_with_value[value];
        int cell_count = cur_cells_with_value.size();
        for(int j = 0; j < cell_count; j++) {
            for(int k = j + 1; k < cell_count; k++) {
                Link* link = nullptr;
                if(cell_count > 2) {
                    link = this->_build_weak_link(cur_cells_with_value[j], cur_cells_with_value[k], value);
                    this->weak_link_between_list[value].push_back(link);
                } else {
                    link = this->_build_strong_link(cur_cells_with_value[j], cur_cells_with_value[k], value);
                    this->strong_link_between_list[value].push_back(link);
                }
            }
        }
    }
}

void LinkManager::_build_box(Box* box){
    using namespace std;
    vector<Cell*> cells;
    for(const auto& cell_row: box->cells) {
        for(const auto& cell: cell_row) {
            cells.push_back(cell);
        }
    }

    map<int, vector<Cell*>> cells_with_value;
    for(int i = 0; i < 9; i++) {
        cells_with_value.insert({i+1, vector<Cell*>()});
    }

    for(const auto& cell: cells) {
        if(cell->filled) continue;
        vector<int> candidates = cell->get_all_candidates();
        for(const auto& candidate: candidates){
            cells_with_value[candidate].push_back(cell);
        }
    }

    for(int i = 0; i < 9; i++) {
        int value = i + 1;
        vector<Cell*> cur_cells_with_value = cells_with_value[value];
        int cell_count = cur_cells_with_value.size();
        for(int j = 0; j < cell_count; j++) {
            Cell* cell1 = cur_cells_with_value[j];
            for(int k = j + 1; k < cell_count; k++) {
                Cell* cell2 = cur_cells_with_value[k];
                if(cell1->row_belong == cell2->row_belong) continue;
                if(cell1->col_belong == cell2->col_belong) continue;
                Link* link = nullptr;
                if(cell_count > 2) {
                    link = this->_build_weak_link(cell1, cell2, value);
                    this->weak_link_between_list[value].push_back(link);
                } else {
                    link = this->_build_strong_link(cell1, cell2, value);
                    this->strong_link_between_list[value].push_back(link);
                }
            }
        }
    }
}

Link* LinkManager::_build_weak_link(Cell* cell1, Cell* cell2, int value){
    LinkNode* node1 = new LinkNode(cell1, value);
    LinkNode* node2 = new LinkNode(cell2, value);

    Link* link = new Link(node1, node2, LINK_WEAK);
    return link;
}

Link* LinkManager::_build_strong_link(Cell* cell1, Cell* cell2, int value){
    LinkNode* node1 = new LinkNode(cell1, value);
    LinkNode* node2 = new LinkNode(cell2, value);

    Link* link = new Link(node1, node2, LINK_STRONG);
    return link;
}