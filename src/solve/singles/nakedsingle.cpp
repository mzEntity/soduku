#include "solve/singles/nakedsingle.h"
#include "soduku.h"
#include "debug_utils/debug_utils.h"

NakedSingleSolver::NakedSingleSolver(Soduku* target){
    this->target = target;
}

Cell* NakedSingleSolver::solve(){
    for(int i = 0; i < 9; i++) {
        for(int j = 0; j < 9; j++) {
            Cell* c = this->target->get_cell(i+1, j+1);
            if(c->filled) continue;
            int candidate_count = c->candidate_count();
            if(candidate_count == 1) {
                this->_work(c);
                return c;
            }
        }
    }

    return nullptr;
}

void NakedSingleSolver::_work(Cell* c){
    ASSERT(!c->filled, "cell has already been filled.");
    ASSERT(c->candidate_count() == 1, "nakedsingle condition unmet.");
    int value = c->get_all_candidates()[0];
    c->fill(value);
}