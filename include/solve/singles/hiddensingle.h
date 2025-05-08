#pragma once

class Soduku;
class Cell;

class HiddenSingleSolver {
    Soduku* target;

   public:
    HiddenSingleSolver(Soduku* target);
    Cell* solve();

    private:
    void _work(Cell* c, int value);
    
    Cell* _solve_row();
    Cell* _solve_column();
    Cell* _solve_box();
};