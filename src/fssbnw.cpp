/**
 * @file fssbnw.cpp
 * @author Matthew Harker
 * @brief Contains the algorithms to run a system through the
 *          FSSNW algorithm
 * @version 1.0
 * @date 2019-05-31
 * 
 * @copyright Copyright (c) 2019
 * 
 */
#include "fssnw.h"


/**
 * @brief Flowshop scheduling with no wait
 * 
 * @param jobs      The matrix of jobsess compTime
 * @param compTime  The resulting matrix of completion compTime
 * @param cols      How many columns to run through the algorithm
 * @return int      The result length the jobs will take
 */
int fssnw(Matrix* jobs, Matrix* compTime)
{
    // loop through the matrix and create each new value
    for (int c = 0; c < jobs->getCols(); ++c)
    {
        for (int r = 0; r < jobs->getRows(); ++r)
        {
            // calculate the new time and assign it
            int time = newTimeFSSNW(jobs, compTime, r, c);
            compTime->setVal(time, r, c);
        }
    }

    // return the final time
    return compTime->getVal(compTime->getRows()-1, compTime->getCols()-1);
}

/**
 * @brief The flowshop scheduling with no wait algorithm. This is a modified version
 *          which runs a permutated job matrix.
 * 
 * @param jobs      The matrix of job run times
 * @param compTime  The matrix of job completion times
 * @param perm      The permutation object
 * @return int      The resulting makespan of the permutation
 */
int fssnwPerm(Matrix* jobs, Matrix* compTime, Permutation* perm)
{
    // retreive the curent permutation size for easier reading
    int curSize = perm->getCurSize();

    // loop through the matrix and create the next values
    for (int c = 0; c < curSize; ++c)
    {
        for (int r = 0; r < jobs->getRows(); ++r)
        {
            // calculate the new time and assign it
            int time = newTimeFSSNWPerm(jobs, compTime, perm, r, c);
            compTime->setVal(time, r, c);
        }
    }

    // return the final time
    return compTime->getVal(compTime->getRows()-1, curSize-1);
}



/**
 * @brief Calculates the next time for the completion compTime matrix.
 *          Specialized for the FSSNW algorithm. 
 * 
 * @param jobs      The matrix of jobsess compTime
 * @param compTime  The matrix of completion compTime
 * @param r         The current row
 * @param c         The current column
 * @return int      The next completion time
 */
int newTimeFSSNW(Matrix* jobs, Matrix* compTime, const int r, const int c)
{
    // edge cases
    if (r == 0 && c == 0) return 0 + jobs->getVal(r, c);
    if (r == 0 && c  > 0) return compTime->getVal(r, c-1) + jobs->getVal(r, c);
    if (r >  0 && c == 0) return compTime->getVal(r-1, c) + jobs->getVal(r, c);


    // if the up job is slower than the left job, adjust all compTime in the column
    int up   = compTime->getVal(r-1, c);
    int left = compTime->getVal(r, c-1);

    if (up < left)
        incrimentPastJobs(compTime, left-up, r, c);

    // return the new value
    return compTime->getVal(r-1, c) + jobs->getVal(r, c);
}

/**
 * @brief Calculates the value for the next job completion time. Uses a
 *          permutated job run time matrix.
 * 
 * @param jobs      The job run time matrix
 * @param compTime  The matrix of job completion times
 * @param perm      The permutation object containing the current job sequence
 * @param r         The row to get the value of
 * @param c         The column to get the value of
 * @return int      The resulting job completion time
 */
int newTimeFSSNWPerm(Matrix* jobs, Matrix* compTime, Permutation* perm, const int r, const int c)
{
    // get the proper indices
    int colInd  = perm->getPerm(c);
    int colIndL = perm->getPerm(0);
    if (c > 0) colIndL = perm->getPerm(c-1); 

    // edge cases
    if (r == 0 && c == 0) return 0 + jobs->getVal(r, colInd);
    if (r == 0 && c  > 0) return compTime->getVal(r, c-1) + jobs->getVal(r, colInd);
    if (r >  0 && c == 0) return compTime->getVal(r-1, c) + jobs->getVal(r, colInd);


    // if the up job is slower than the left job, adjust all compTime in the column
    int up   = compTime->getVal(r-1, c);
    int left = compTime->getVal(r, c-1);

    if (up < left)
        incrimentPastJobs(compTime, left-up, r, c);

    // return the new value
    return compTime->getVal(r-1, c) + jobs->getVal(r, colInd);
}

/**
 * @brief Incriments the completion time of previous jobs for the same machine
 * 
 * @param compTime  The matrix of completion compTime
 * @param diff      The value to incriment the prior jobs by
 * @param curRow    The current row, incriments all prior elements above this row
 * @param c         The current column
 */
void incrimentPastJobs(Matrix* compTime, int diff, const int curRow, const int c)
{
    for (int r = 0; r < curRow; ++r)
    {
        int newVal = compTime->getVal(r, c) + diff;
        compTime->setVal(newVal, r, c);
    }
}

