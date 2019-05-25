
#ifndef FLOWSHOP_H
#define FLOWSHOP_H

#include "Matrix.h"

int fss  (Matrix* processTimes, Matrix* finalTimes);
int fssb (Matrix* processTimes, Matrix* finalTimes);
int fssnw(Matrix* processTimes, Matrix* finalTimes);

int baseTimeFSS (Matrix* completionTimes, const int row, const int col);
int newTimeFSSB (Matrix* processTimes, Matrix* finalTimes, const int row, const int col);
int newTimeFSSNW(Matrix* processTimes, Matrix* finalTimes, const int row, const int col);

void incrimentPastJobs(Matrix* finalTimes, int difference, const int curRow, const int col);

#endif
