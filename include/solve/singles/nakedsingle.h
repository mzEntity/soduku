#pragma once

class Soduku;
class Cell;

class NakedSingleSolver {
    Soduku* target;

   public:
    NakedSingleSolver(Soduku* target);
    Cell* solve();

    private:
    void _work(Cell* c);
};