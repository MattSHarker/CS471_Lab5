
#ifndef MEMORY_H
#define MEMORY_H

// #include <ctime>

class Memory {
private:
    int    funcCalls;   // how many function calls the algorithm used

    clock_t timer;      // stores the clock values of the start/stop times
    double  timeTaken;  // how long the algorithm took to execute

public:
    Memory();

    // functions for funcCalls
    int  getFuncCalls();
    void incrFuncCalls();

    // functions for timeTaken
    void   startTimer();
    void   stopTimer();
    void   setTimeTaken(const double time);
    double getTimeTaken();

    // overall functions
    void writeAllData(Matrix* jobs, Matrix* compTimes, Permutation* perm, const int alg, const int datafile);
    void writeRawData(Matrix* jobTimes, Matrix* complTimes, Permutation* perm, const int alg, const int datafile);
    void writeGanttData(Matrix* jobs, Matrix* compTimes, Permutation* perm, const int alg, const int datafile);

};

#endif
