
#ifndef MATRIX_H
#define MATRIX_H

#include <string>

#include "Permutation.h"

using namespace std;

class Matrix {
private:
    int cols;
    int rows;

    int*  jobCosts;     // holds the total cost of each column
    int** matrix;       // [rows][cols]

public:
    Permutation* perm;  // public to make using its functions easier

    // constructors and destructors
    Matrix(const int row, const int col);
    Matrix(string fileName);
    Matrix(int fileName);

    ~Matrix();

    // functions for constants
    int getCols();
    int getRows();

    // functions for jobCosts
    void generateJobCosts();
    int  getJobCost(const int job);
    int* getJobCosts();

    // functions for matrix
    int  getVal(const int row, const int col);
    void setVal(int newVal, const int row, const int col);

    // functions for perm
    void generateBestPermutation();
    void initAllJobs();
    void sortAllJobs();

    // misc functions
    void resize(const int newRows, const int newCols);
    void print();
    void printByPerm();
};

#endif
