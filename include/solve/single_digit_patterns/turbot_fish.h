#pragma once

class Sudoku;
class Cell;
class LinkManager;
class LinkNode;

class TurbotFishSolver {
    Sudoku* target;
    LinkManager* linkManager;

   public:
    TurbotFishSolver(Sudoku* target, LinkManager* linkManager);
    bool solve();


   private:
    bool _solve_node(LinkNode* node);
};