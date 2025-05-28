#pragma once

class Sudoku;
class Cell;

class HiddenSingleSolver {
    Sudoku* target;

   public:
    HiddenSingleSolver(Sudoku* target);
    Cell* solve();

    private:
    void _work(Cell* c, int value);
    
    Cell* _solve_row();
    Cell* _solve_column();
    Cell* _solve_box();
};