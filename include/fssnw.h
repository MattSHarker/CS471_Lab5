
#ifndef FSSNW_H
#define FSSNW_H

#include "Matrix.h"
#include "Permutation.h"

int fssnw(Matrix* jobTimes, Matrix* compTimes);
int fssnwPerm(Matrix* jobTimes, Matrix* compTimes, Permutation* perm);

int newTimeFSSNW(Matrix* jobs, Matrix* compTimes, const int row, const int col);
int newTimeFSSNWPerm(Matrix* jobs, Matrix* compTimes, Permutation* perm, const int row, const int col);

void incrimentPastJobs(Matrix* compTimes, int difference, const int curRow, const int col);

#endif
