#pragma once

class Soduku;
class Cell;
class Box;

class LockedCandidatesPointing {
    Soduku* target;

   public:
    LockedCandidatesPointing(Soduku* target);
    bool solve();

    private:
    bool _solve_box(Box* b);
};