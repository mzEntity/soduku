#pragma once

class Sudoku;

class PeerPruner {
    Sudoku* target;

   public:
    PeerPruner(Sudoku* target);
    void prune(int row_num, int col_num);
    void prune();
};