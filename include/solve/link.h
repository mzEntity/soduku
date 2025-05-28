#pragma once

#include <map>
#include <vector>

#define LINK_STRONG 1
#define LINK_WEAK 2

typedef int link_type;

class Sudoku;
class Cell;

class Row;
class Column;
class Box;

class LinkNode;
class Link;
class LinkManager;

class LinkNode {
   public:
    int candidate;
    Cell* cell;
    std::vector<Link*> strong_link_between_list;
    std::vector<Link*> weak_link_between_list;

    LinkNode(Cell* cell, int candidate);

    std::vector<Link*> get_strong_link_between_list() {
        return this->strong_link_between_list;
    }

    std::vector<Link*> get_weak_link_between_list() {
        return this->weak_link_between_list;
    }

    std::vector<LinkNode*> get_node_strong_linked_between();
    std::vector<LinkNode*> get_node_linked_between();
};

class Link {
   private:
   public:
    LinkNode* node1;
    LinkNode* node2;

    link_type type;

    Link(LinkNode* node1, LinkNode* node2, link_type type);

    std::string to_string();
};

class LinkManager {
   private:
    Sudoku* target;

    std::vector<std::vector<std::vector<LinkNode*>>> all_nodes;

    std::map<int, std::vector<Link*>> strong_link_between_list;
    std::map<int, std::vector<Link*>> weak_link_between_list;

   public:
    LinkManager(Sudoku* target);
    void build();
    void print_all_links();

    std::vector<LinkNode*> get_nodes(int row_num, int col_num);

    ~LinkManager() { this->_clear_all_build(); }

   private:
    void _build_row(Row* row);
    void _build_column(Column* column);
    void _build_box(Box* box);

    Link* _build_weak_link_between(Cell* cell1, Cell* cell2, int value);
    Link* _build_strong_link_between(Cell* cell1, Cell* cell2, int value);

    void _clear_all_build();
    LinkNode* _get_node(Cell* cell, int value);
};