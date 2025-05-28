#pragma once

class Sudoku;
class Cell;

class NakedSingleSolver {
    Sudoku* target;

   public:
    NakedSingleSolver(Sudoku* target);
    Cell* solve();

    private:
    void _work(Cell* c, int value);
};