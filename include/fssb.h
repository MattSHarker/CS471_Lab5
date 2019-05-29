
#ifndef FSSB_H
#define FSSB_H

#include "Matrix.h"
#include "Permutation.h"

int fssb(Matrix* jobTimes, Matrix* compTimes);
int fssbPerm(Matrix* jobTimes, Matrix* compTimes, Permutation* perm);

int newTimeFSSB(Matrix* jobs, Matrix* compTimes, const int r, const int c);
int newTimeFSSBPerm(Matrix* jobTimes, Matrix* compTimes, Permutation* perm, const int r, const int c);

#endif
