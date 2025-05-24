#include "debug_utils/debug_utils.h"

#include "solve/link.h"
#include <map>
#include <vector>
#include "house.h"
#include "soduku.h"

LinkNode::LinkNode(Cell* cell, int candidate) {
    this->cell = cell;
    this->candidate = candidate;
}

std::vector<LinkNode*> LinkNode::get_node_strong_linked_between() {
    using namespace std;
    vector<LinkNode*> result;
    for(Link* link: this->strong_link_between_list) {
        LinkNode* other_node = link->node1;
        if(other_node == this) {
            other_node = link->node2;
        }
        ASSERT(other_node != this, "strong link between with self");
        result.push_back(other_node);
    }
    return result;
}

std::vector<LinkNode*> LinkNode::get_node_linked_between() {
    using namespace std;
    vector<LinkNode*> result;
    for(Link* link: this->strong_link_between_list) {
        LinkNode* other_node = link->node1;
        if(other_node == this) {
            other_node = link->node2;
        }
        ASSERT(other_node != this, "strong link between with self");
        result.push_back(other_node);
    }

    for(Link* link: this->weak_link_between_list) {
        LinkNode* other_node = link->node1;
        if(other_node == this) {
            other_node = link->node2;
        }
        ASSERT(other_node != this, "weak link between with self");
        result.push_back(other_node);
    }
    return result;
}

Link::Link(LinkNode* node1, LinkNode* node2, link_type type) {
    this->node1 = node1;
    this->node2 = node2;
    this->type = type;
}

std::string Link::to_string() {
    int value = this->node1->candidate;
    return this->node1->cell->get_position() + "--" + std::to_string(value) +
           "--" + this->node2->cell->get_position();
}

LinkManager::LinkManager(Soduku* target) {
    this->target = target;
    for (int i = 0; i < 9; i++) {
        int value = i + 1;
        this->strong_link_between_list.insert({value, std::vector<Link*>()});
        this->weak_link_between_list.insert({value, std::vector<Link*>()});
    }

    this->all_nodes = std::vector<std::vector<std::vector<LinkNode*>>>(9, std::vector<std::vector<LinkNode*>>(9, std::vector<LinkNode*>()));
}

void LinkManager::build() {
    using namespace std;
    this->_clear_all_build();

    for(int i = 0; i < 9; i++) {
        Row* row = this->target->rows[i];
        for(int j = 0; j < 9; j++) {
            Cell* cell = row->cells[j];
            if(cell->filled) continue;
            vector<int> candidates = cell->get_all_candidates();
            for(const auto& candidate: candidates) {
                LinkNode* node = new LinkNode(cell, candidate);
                this->all_nodes[i][j].push_back(node);
            }
        }
    }

    for (const auto& row : this->target->rows) {
        this->_build_row(row);
    }

    for (const auto& column : this->target->columns) {
        this->_build_column(column);
    }

    for (const auto& box_row : this->target->boxes) {
        for (const auto& box : box_row) {
            this->_build_box(box);
        }
    }
}

void LinkManager::_clear_all_build() {
    using namespace std;
    for(int i = 0; i < 9; i++) {
        for(int j = 0; j < 9; j++) {
            vector<LinkNode*> nodes = this->all_nodes[i][j];
            for(LinkNode* node: nodes) {
                node->strong_link_between_list.clear();
                node->weak_link_between_list.clear();
                delete node;
            }
            this->all_nodes[i][j].clear();
        }
    }
    

    for (auto& pair : this->strong_link_between_list) {
        for (Link* link : pair.second) {
            delete link;
        }
        pair.second.clear();
    }

    for (auto& pair : this->weak_link_between_list) {
        for (Link* link : pair.second) {
            delete link;
        }
        pair.second.clear();
    }
}

void LinkManager::print_all_links() {
    using namespace std;
    cout << "strong links: " << endl;
    for (int i = 0; i < 9; i++) {
        int value = i + 1;
        vector<Link*> strong_links = this->strong_link_between_list[value];
        if (strong_links.size() == 0)
            continue;
        cout << "\tvalue " << value << ": " << endl;
        for (const auto& link : strong_links) {
            cout << "\t\t" << link->to_string() << endl;
        }
    }

    cout << "weak links: " << endl;
    for (int i = 0; i < 9; i++) {
        int value = i + 1;
        vector<Link*> weak_links = this->weak_link_between_list[value];
        if (weak_links.size() == 0)
            continue;
        cout << "\tvalue " << value << ": " << endl;
        for (const auto& link : weak_links) {
            cout << "\t\t" << link->to_string() << endl;
        }
    }
}

std::vector<LinkNode*> LinkManager::get_nodes(int row_num, int col_num){
    ASSERT(row_num >= 1 && row_num <= 9,
           "Row number should be between 1 and 9.");
    ASSERT(col_num >= 1 && col_num <= 9,
           "Col number should be between 1 and 9.");
    return this->all_nodes[row_num - 1][col_num - 1];
}

void LinkManager::_build_row(Row* row) {
    using namespace std;
    vector<Cell*> cells = row->cells;
    map<int, vector<Cell*>> cells_with_value;
    for (int i = 0; i < 9; i++) {
        cells_with_value.insert({i + 1, vector<Cell*>()});
    }

    for (const auto& cell : cells) {
        if (cell->filled)
            continue;
        vector<int> candidates = cell->get_all_candidates();
        for (const auto& candidate : candidates) {
            cells_with_value[candidate].push_back(cell);
        }
    }

    for (int i = 0; i < 9; i++) {
        int value = i + 1;
        vector<Cell*> cur_cells_with_value = cells_with_value[value];
        int cell_count = cur_cells_with_value.size();
        for (int j = 0; j < cell_count; j++) {
            for (int k = j + 1; k < cell_count; k++) {
                Link* link = nullptr;
                if (cell_count > 2) {
                    link =
                        this->_build_weak_link_between(cur_cells_with_value[j],
                                               cur_cells_with_value[k], value);
                    this->weak_link_between_list[value].push_back(link);
                } else {
                    link = this->_build_strong_link_between(cur_cells_with_value[j],
                                                    cur_cells_with_value[k],
                                                    value);
                    this->strong_link_between_list[value].push_back(link);
                }
            }
        }
    }
}

void LinkManager::_build_column(Column* column) {
    using namespace std;
    vector<Cell*> cells = column->cells;
    map<int, vector<Cell*>> cells_with_value;
    for (int i = 0; i < 9; i++) {
        cells_with_value.insert({i + 1, vector<Cell*>()});
    }

    for (const auto& cell : cells) {
        if (cell->filled)
            continue;
        vector<int> candidates = cell->get_all_candidates();
        for (const auto& candidate : candidates) {
            cells_with_value[candidate].push_back(cell);
        }
    }

    for (int i = 0; i < 9; i++) {
        int value = i + 1;
        vector<Cell*> cur_cells_with_value = cells_with_value[value];
        int cell_count = cur_cells_with_value.size();
        for (int j = 0; j < cell_count; j++) {
            for (int k = j + 1; k < cell_count; k++) {
                Link* link = nullptr;
                if (cell_count > 2) {
                    link =
                        this->_build_weak_link_between(cur_cells_with_value[j],
                                               cur_cells_with_value[k], value);
                    this->weak_link_between_list[value].push_back(link);
                } else {
                    link = this->_build_strong_link_between(cur_cells_with_value[j],
                                                    cur_cells_with_value[k],
                                                    value);
                    this->strong_link_between_list[value].push_back(link);
                }
            }
        }
    }
}

void LinkManager::_build_box(Box* box) {
    using namespace std;
    vector<Cell*> cells;
    for (const auto& cell_row : box->cells) {
        for (const auto& cell : cell_row) {
            cells.push_back(cell);
        }
    }

    map<int, vector<Cell*>> cells_with_value;
    for (int i = 0; i < 9; i++) {
        cells_with_value.insert({i + 1, vector<Cell*>()});
    }

    for (const auto& cell : cells) {
        if (cell->filled)
            continue;
        vector<int> candidates = cell->get_all_candidates();
        for (const auto& candidate : candidates) {
            cells_with_value[candidate].push_back(cell);
        }
    }

    for (int i = 0; i < 9; i++) {
        int value = i + 1;
        vector<Cell*> cur_cells_with_value = cells_with_value[value];
        int cell_count = cur_cells_with_value.size();
        for (int j = 0; j < cell_count; j++) {
            Cell* cell1 = cur_cells_with_value[j];
            for (int k = j + 1; k < cell_count; k++) {
                Cell* cell2 = cur_cells_with_value[k];
                if (cell1->row_belong == cell2->row_belong)
                    continue;
                if (cell1->col_belong == cell2->col_belong)
                    continue;
                Link* link = nullptr;
                if (cell_count > 2) {
                    link = this->_build_weak_link_between(cell1, cell2, value);
                    this->weak_link_between_list[value].push_back(link);
                } else {
                    link = this->_build_strong_link_between(cell1, cell2, value);
                    this->strong_link_between_list[value].push_back(link);
                }
            }
        }
    }
}

Link* LinkManager::_build_weak_link_between(Cell* cell1, Cell* cell2, int value) {
    LinkNode* node1 = this->_get_node(cell1, value);
    LinkNode* node2 = this->_get_node(cell2, value);

    Link* link = new Link(node1, node2, LINK_WEAK);

    node1->weak_link_between_list.push_back(link);
    node2->weak_link_between_list.push_back(link);
    return link;
}

Link* LinkManager::_build_strong_link_between(Cell* cell1, Cell* cell2, int value) {
    LinkNode* node1 = this->_get_node(cell1, value);
    LinkNode* node2 = this->_get_node(cell2, value);

    Link* link = new Link(node1, node2, LINK_STRONG);
    node1->strong_link_between_list.push_back(link);
    node2->strong_link_between_list.push_back(link);
    return link;
}

LinkNode* LinkManager::_get_node(Cell* cell, int value){
    int row_index = cell->row_belong->num - 1;
    int col_index = cell->col_belong->num - 1;
    for(LinkNode* node: this->all_nodes[row_index][col_index]) {
        if(node->candidate == value) return node;
    }
    LOG_WARN("linknode not found");
    return nullptr;
}