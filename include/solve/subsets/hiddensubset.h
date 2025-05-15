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

class HiddenSubsetSolver {
    Soduku* target;

   public:
    HiddenSubsetSolver(Soduku* target);
    bool solve();

   private:
    bool _solve_row(Row* row, int length);
    bool _solve_column(Column* column, int length);
    bool _solve_box(Box* box, int length);

    bool _work(std::set<Cell*> subset, std::set<int> values);
    std::pair<std::set<Cell*>, std::set<int>> _hidden_subset(
    std::vector<Cell*> cells,
    int length);


};