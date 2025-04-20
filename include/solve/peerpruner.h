#pragma once

class Soduku;

class PeerPruner {
    Soduku* target;

   public:
    PeerPruner(Soduku* target);
    void prune(int row_num, int col_num);
    void prune();
};