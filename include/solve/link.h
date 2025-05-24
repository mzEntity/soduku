#pragma once


#include <vector>
#include <map>

#define LINK_STRONG 1
#define LINK_WEAK 2

typedef int link_type;

class Soduku;
class Cell;

class Row;
class Column;
class Box;

class LinkNode {
   public:
    int candidate;
    Cell* cell;
    LinkNode(Cell* cell, int candidate);
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
    Soduku* target;

    std::map<int, std::vector<Link*>> strong_link_between_list;
    std::map<int, std::vector<Link*>> weak_link_between_list;

   public:
    LinkManager(Soduku* target);
    void build();
    void print_all_links();

   private:
    void _build_row(Row* row);
    void _build_column(Column* column);
    void _build_box(Box* box);

    Link* _build_weak_link(Cell* cell1, Cell* cell2, int value);
    Link* _build_strong_link(Cell* cell1, Cell* cell2, int value);
};