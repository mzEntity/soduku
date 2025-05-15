#include "solve/singles/nakedsingle.h"
#include "soduku.h"
#include "debug_utils/debug_utils.h"
#include "solve/solutionlog.h"

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
                int value = c->get_all_candidates()[0];
                this->_work(c, value);
                SOLUTION_INFO("NakedSingle: " + c->get_position() + "=" + std::to_string(value));
                return c;
            }
        }
    }

    return nullptr;
}

void NakedSingleSolver::_work(Cell* c, int value){
    c->fill(value);
}