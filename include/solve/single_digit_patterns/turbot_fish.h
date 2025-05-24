#pragma once

class Soduku;
class Cell;
class LinkManager;
class LinkNode;

class TurbotFishSolver {
    Soduku* target;
    LinkManager* linkManager;

   public:
    TurbotFishSolver(Soduku* target, LinkManager* linkManager);
    bool solve();


   private:
    bool _solve_node(LinkNode* node);
};