#include "solve/peerpruner.h"
#include "sudoku.h"
#include "debug_utils/debug_utils.h"

PeerPruner::PeerPruner(Sudoku* target){
    this->target = target;
}

void PeerPruner::prune(int row_num, int col_num){
    Cell* src = this->target->get_cell(row_num, col_num);

    if(!src->filled) return;
    Row* row_belong = src->row_belong;
    Column* col_belong = src->col_belong;
    Box* box_belong = src->box_belong;
    ASSERT(row_belong && col_belong && box_belong, "belong is nullptr");
    row_belong->remove_candidate_from(src);
    col_belong->remove_candidate_from(src);
    box_belong->remove_candidate_from(src);
}

void PeerPruner::prune(){
    for(int i = 0; i < 9; i++) {
        for(int j = 0; j < 9; j++) {
            this->prune(i + 1, j + 1);
        }
    }
}