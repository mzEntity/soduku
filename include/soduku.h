#ifndef SODUKU_H
#define SODUKU_H

#include <vector>
using namespace std;

class Cell {
    static const int POSSIBLE_COUNT = 9;
public:
    vector<bool> candidates;
    int value;
    Cell(){
        this->candidates = vector<bool>(this->POSSIBLE_COUNT, true);
        this->value = -1;
    }

    void fill(int value){
        
    }
};

const int Cell::POSSIBLE_COUNT;


class Soduku {
    static const int ROW_COUNT = 9;
    static const int COL_COUNT = 9;
public:
    vector<vector<Cell>> cells;

    Soduku(){
        this->cells = vector<vector<Cell>>(this->ROW_COUNT, vector<Cell>(this->COL_COUNT, Cell()));
    }

    void fill(vector<vector<int>> quest) {
        
    }
};

const int Soduku::ROW_COUNT;
const int Soduku::COL_COUNT;

#endif