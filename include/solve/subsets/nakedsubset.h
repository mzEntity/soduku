#pragma once

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
};