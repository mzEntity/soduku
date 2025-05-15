#pragma once
#include <queue>
#include <set>
#include <utility>
#include <vector>

class Soduku;
class Cell;
class Row;
class Column;
class Box;

class NakedSubsetSolver {
    Soduku* target;

   public:
    NakedSubsetSolver(Soduku* target);
    bool solve();

   private:
    bool _solve_row(Row* row);
    bool _solve_column(Column* column);
    bool _solve_box(Box* box);

    std::pair<std::set<Cell*>, std::set<int>> _naked_subset(
        std::vector<Cell*> cells,
        int length);

    bool _work(std::set<Cell*> subset, std::set<int> values);
};