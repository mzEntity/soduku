#pragma once

class Sudoku;
class Cell;
class Box;
class Row;
class Column;

class LockedCandidatesPointing {
    Sudoku* target;

   public:
    LockedCandidatesPointing(Sudoku* target);
    bool solve();

    private:
    bool _solve_box(Box* b);
};


class LockedCandidatesClaiming {
    Sudoku* target;

   public:
    LockedCandidatesClaiming(Sudoku* target);
    bool solve();

    private:
    bool _solve_row(Row* r);
    bool _solve_column(Column* c);
};