
#ifndef PERMUTATION_H
#define PERMUTATION_H

#include <vector>

class Permutation {
private:
    int size;       // the size of the permutation (number of jobs (cols))
    int curSize;    // how many elements are being used
    int pos;        // the position of the most recent addition

    int bestVal;    // the value of the (current) best permutation

    int* perm;      // holds the current permutation
    int* best;      // the (current) best permutation

    int** allJobs;   // holds the indices of each job in from worst to best cost
                        // [0][x] is for the job index
                        // [1][y] is for job cost 

public:
    Permutation(const int newSize); // size is # cols in the matrix
    ~Permutation();

    // functions for pos
    int  getPos();
    void setPos(const int newPos);
    void nextPos();

    // functions for size
    int  getSize();

    // functions for curSize
    int  getCurSize();
    void setCurSize(const int newCurSize);
    void incrimentCurSize();

    // functions for bestVal
    int  getBestVal();
    void setBestVal(const int newVal);

    // functions for perm
    int  getPerm(const int elem);
    int* getPerm();
    void nextPermutation();
    void addElement(const int newElement);
    void resize(const int newSize);

    // functions for best
    int  getBest(const int elem);
    int* getBest();
    void setCurrentToBest();
    void setBestToCurrent();

    // functions for allJobs
    int  getJobOrder(const int elem);
    int  getJobIndex(const int elem);
    int* getJobOrder();
    void setAllJobs(const int* costs);
    void sortAllJobs();
    void printAllJobs();

};

#endif
