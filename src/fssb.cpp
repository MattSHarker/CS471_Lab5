
#include "fssb.h"

/**
 * @brief Flowshop scheduling with blocking
 * 
 * @param jobs      The matrix of jobsess times
 * @param compTime  The resulting matrix of completion times
 * @param cols      How many columns to run through the algorithm
 * @return int      The result length the jobs will take
 */
int fssb(Matrix* jobs, Matrix* compTime)
{
    // loop through the matrix and create each new value
    for (int c = 0; c < jobs->getCols(); ++c)
    {
        for (int r = 0; r < jobs->getRows(); ++r)
        {
            // calculate the new time and assign it
            int time = newTimeFSSB(jobs, compTime, r, c);
            compTime->setVal(time, r, c);
        }
    }

    // return the final time
    return compTime->getVal(compTime->getRows()-1,  compTime->getCols()-1);
}

int fssbPerm(Matrix* jobs, Matrix* compTime, Permutation* perm)
{
    // retreive the curent permutation size for easier reading
    int curSize = perm->getCurSize();

    // loop through the matrix and create the next values
    for (int c = 0; c < curSize; ++c)
    {
        for (int r = 0; r < jobs->getRows(); ++r)
        {
            // calculate the new time and assign it
            int time = newTimeFSSBPerm(jobs, compTime, perm, r, c);
            compTime->setVal(time, r, c);
        }
    }

    // return the final time
    return compTime->getVal(compTime->getRows()-1, curSize-1);
}


/**
 * @brief Calculates the next completion time for the compTime matrix.
 *          Specialized for the FSSB algorithm
 * 
 * @param jobs      The matrix of jobsess compTime
 * @param compTime  The matrix oint newTimeFSSNWPerm()f completion compTime
 * @param r         The current row
 * @param c         The current column
 * @return int      The next completion time
 */
int newTimeFSSB(Matrix* jobs, Matrix* compTime, const int r, const int c)
{
    // edge case for c == 0
    if (r == 0 && c == 0) return 0 + jobs->getVal(r, c);
    if (r >  0 && c == 0) return compTime->getVal(r-1, c) + jobs->getVal(r, c);
    
    // edge case for r == 0, c > 0
    if (r == 0 && c > 0)
    {
        int val1 = compTime->getVal(r, c-1) + jobs->getVal(r, c);
        int val2 = compTime->getVal(r+1, c-1); 
        return max(val1, val2);
    }

    // edge case for r == numRows, c > 0
    if (r == compTime->getRows()-1 && c > 0)
        return compTime->getVal(r-1, c) + jobs->getVal(r, c);

    // if not an edge case, then return the max of the values to the left and above
    return max(compTime->getVal(r+1, c-1), compTime->getVal(r-1, c) + jobs->getVal(r, c));
}



int newTimeFSSBPerm(Matrix* jobs, Matrix* compTime, Permutation* perm, const int r, const int c)
{
    // get the proper indices
    int colInd  = perm->getPerm(c);
    int colIndL = perm->getPerm(0);
    if (c > 0) colIndL = perm->getPerm(c-1);    // ensures colIndL does not access a negative index

    // edge case for r == numRows-1, c > 0
    if (r == compTime->getRows()-1 && c > 0)
        return compTime->getVal(r-1, c) + jobs->getVal(r, colInd);

    // edge cases for c == 0
    if (r == 0 && c == 0) return jobs->getVal(r, colInd);
    if (r  > 0 && c == 0) return compTime->getVal(r-1, c) + jobs->getVal(r, colInd);
    
    // edge case for r == 0, c > 0
    if (r == 0 && c > 0)
    {
        int val1 = compTime->getVal(0, c-1) + jobs->getVal(0, colInd);
        int val2 = compTime->getVal(1, c-1); 
        return max(val1, val2);
    }

    // if not an edge case, then return the max of the values to the left and above
    int val1 = compTime->getVal(r+1, c-1);
    int val2 = compTime->getVal(r-1, c) + jobs->getVal(r, colInd);
    return max(val1, val2);
}
