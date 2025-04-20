#ifndef SODUKU_H
#define SODUKU_H

#include <vector>
#include <iostream>

class Cell {
    static const int POSSIBLE_COUNT;

   public:
    std::vector<bool> candidates;
    int value;
    bool filled;
    bool fixed;

    Cell();

    void fix(int value);
    void fill(int value);

    bool have_candidate(int value);

    void remove_candidate(int num);

   private:
    void _remove_all_candidates();
    void _remove_all_candidates_except(int value);
};



class Soduku {
    static const int ROW_COUNT;
    static const int COL_COUNT;

   public:
    std::vector<std::vector<Cell>> cells;

    Soduku();

    void init(std::vector<std::vector<int>> quest);

    void print(std::ostream& out);
};


#endif