
#ifndef MEMORY_H
#define MEMORY_H

class Memory {
private:
    int funcCalls;      // how many function calls the algorithm used
    int originalFit;    // the original non-permutated fitness
    int bestFit;        // the best permutated fitness

    int size;               // the size of bestPermutation
    int* bestPermutation;   // the best permutation (stores job indices)

public:
    Memory();
    Memory(const int size);     // size is determined by the number of jobs (columns)
    ~Memory();

    // functions for funcCalls
    int  getFuncCalls();
    void incrFuncCalls();

    // functions for originalFit
    int  getOriginalFit();
    void setOriginalFit(const int newFitness);

    // functions for bestFit
    int  getBestFit();
    void setBestFit(const int newFitness);

    // functions for size
    int getSize();

    // functions for bestPermutation
    int  getBestPermutation(const int elem);
    int* getBestPermutation();
    void setBestPermutation(int newVal, const int elem);
    void setBestPermutation(int* newPerm);
    void resizeBestPermutation(const int newSize);

};

#endif
