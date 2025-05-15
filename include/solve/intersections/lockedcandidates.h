#pragma once

class Soduku;
class Cell;
class Box;
class Row;
class Column;

class LockedCandidatesPointing {
    Soduku* target;

   public:
    LockedCandidatesPointing(Soduku* target);
    bool solve();

    private:
    bool _solve_box(Box* b);
};


class LockedCandidatesClaiming {
    Soduku* target;

   public:
    LockedCandidatesClaiming(Soduku* target);
    bool solve();

    private:
    bool _solve_row(Row* r);
    bool _solve_column(Column* c);
};