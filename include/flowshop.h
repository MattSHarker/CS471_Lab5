
#ifndef FLOWSHOP_H
#define FLOWSHOP_H

#include "Matrix.h"
#include "Memory.h"
#include "Permutation.h"

void run();
void runFlowshop();
void runCustomPermutation();
int  flowshop(const int datafile, const int alg);

int fssType    (Matrix* jobs, Matrix* comp, const int alg);
int fssTypePerm(Matrix* jobs, Matrix* comp, Permutation* perm, const int alg);

int fssPerm  (Matrix* processTimes, Matrix* finalTimes, Permutation* perm);
int fssbPerm (Matrix* processTimes, Matrix* finalTimes, Permutation* perm);
int fssnwPerm(Matrix* processTimes, Matrix* finalTimes, Permutation* perm);


int fss  (Matrix* processTimes, Matrix* finalTimes);
int fssb (Matrix* processTimes, Matrix* finalTimes);
int fssnw(Matrix* processTimes, Matrix* finalTimes);

int baseTimeFSS (Matrix* completionTimes, const int row, const int col);
int newTimeFSSB (Matrix* processTimes, Matrix* finalTimes, const int row, const int col);
int newTimeFSSNW(Matrix* processTimes, Matrix* finalTimes, const int row, const int col);

void incrimentPastJobs(Matrix* finalTimes, int difference, const int curRow, const int col);

int baseTimeFSSPerm (Matrix* jobs, Matrix* compTime, Permutation* perm, const int r, const int c);
int newTimeFSSBPerm (Matrix* jobs, Matrix* compTime, Permutation* perm, const int r, const int c);
int newTimeFSSNWPerm(Matrix* jobs, Matrix* compTime, Permutation* perm, const int r, const int c);

void initialize(Matrix* jobTimes, Permutation* perm);
void initParameters(int &start, int &end, int &algStart, int &algEnd);

#endif
