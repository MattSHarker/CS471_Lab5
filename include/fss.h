
#ifndef FSS_H
#define FSS_H

#include "Matrix.h"
#include "Permutation.h"

int fss    (Matrix* jobTimes, Matrix* compTimes);
int fssPerm(Matrix* jobsTimes, Matrix* compTimes, Permutation* perm);

int baseTimeFSS(Matrix* compTimes, const int row, const int col);
int baseTimeFSSPerm(Matrix* jobTimes, Matrix* compTimes, Permutation* perm, const int r, const int c);

#endif
