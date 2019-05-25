/**
 * @file flowshop.cpp
 * @author Matthew Harker
 * @brief Contains three flow shop algorithms: FSS, FSSB, and FSSNW.
            Takes in a matrix of process times, and creates another
            matrix of completion times for each job/machine.

 * @version 1.0
 * @date 2019-05-31
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#include "flowshop.h"

/**
 * @brief The basic flowshop scheduling algorithm
 * 
 * @param proc      The matrix of process times
 * @param times     The resulting matrix of completion times
 * @return int      The result length the jobs will take
 */
int fss(Matrix* proc, Matrix* times)
{
    // loop through the matrix and create the next values
    for (int c = 0; c < proc->getCols(); ++c)
    {
        for (int r = 0; r < proc->getRows(); ++r)
        {
            // calculate the new time and assign it
            int time = baseTimeFSS(times, r, c) + proc->getVal(r, c);
            times->setVal(time, r, c);
        }
    }

    // return the final time
    return times->getVal(times->getRows() - 1, times->getCols() - 1);
}

/**
 * @brief Flowshop scheduling with blocking
 * 
 * @param proc      The matrix of process times
 * @param times     The resulting matrix of completion times
 * @return int      The result length the jobs will take
 */
int fssb(Matrix* proc, Matrix* times)
{
    // loop through the matrix and create each new value
    for (int c = 0; c < proc->getCols(); ++c)
    {
        for (int r = 0; r < proc->getRows(); ++r)
        {
            // calculate the new time and assign it
            int time = newTimeFSSB(proc, times, r, c);
            times->setVal(time, r, c);
        }
    }

    // return the final time
    return times->getVal(times->getRows() - 1, times->getCols() - 1);
}

/**
 * @brief Flowshop scheduling with no wait
 * 
 * @param proc      The matrix of process times
 * @param times     The resulting matrix of completion times
 * @return int      The result length the jobs will take
 */
int fssnw(Matrix* proc, Matrix* times)
{
    // loop through the matrix and create each new value
    for (int c = 0; c < proc->getCols(); ++c)
    {
        for (int r = 0; r < proc->getRows(); ++r)
        {
            // calculate the new time and assign it
            int time = newTimeFSSNW(proc, times, r, c);
            times->setVal(time, r, c);
        }
    }

    // return the final time
    return times->getVal(times->getRows() - 1, times->getCols() - 1);
}

/**
 * @brief Calculates the base time for the next job completion time.
 *          Specialized for the basic FFS algorithm.
 * 
 * @param times The matrix of completion times
 * @param r     The current row of the matrices
 * @param c     The current column of the matrices
 * @return int  The base time of the next completion time
 */
int baseTimeFSS(Matrix* times, const int r, const int c)
{
    // check for edge scenarios
    if (r == 0 && c == 0) return 0;
    if (r == 0 && c  > 0) return times->getVal(r, c-1);
    if (r >  0 && c == 0) return times->getVal(r-1, c);
    
    // if not an edge case, then return the max of the values to the left and above
    return max(times->getVal(r, c-1), times->getVal(r-1, c));
}

/**
 * @brief Calculates the next completion time for the times matrix.
 *          Specialized for the FSSB algorithm
 * 
 * @param proc  The matrix of process times
 * @param times The matrix of completion times
 * @param r     The current row
 * @param c     The current column
 * @return int  The next completion time
 */
int newTimeFSSB(Matrix* proc, Matrix* times, const int r, const int c)
{
    // edge case for c == 0
    if (r == 0 && c == 0) return 0 + proc->getVal(r, c);
    if (r >  0 && c == 0) return times->getVal(r-1, c) + proc->getVal(r, c);
    
    // edge case for r == 0, c > 0
    if (r == 0 && c > 0)
    {
        int val1 = times->getVal(r, c-1) + proc->getVal(r, c);
        int val2 = times->getVal(r+1, c-1); 
        return max(val1, val2);
    }

    // edge case for r == numRows, c > 0
    if (r == times->getRows()-1 && c > 0)
        return times->getVal(r-1, c) + proc->getVal(r, c);

    // if not an edge case, then return the max of the values to the left and above
    return max(times->getVal(r+1, c-1), times->getVal(r-1, c) + proc->getVal(r, c));
}

/**
 * @brief Calculates the next time for the completion times matrix.
 *          Specialized for the FSSNW algorithm. 
 * 
 * @param proc  The matrix of process times
 * @param times The matrix of completion times
 * @param r     The current row
 * @param c     The current column
 * @return int  The next completion time
 */
int newTimeFSSNW(Matrix* proc, Matrix* times, const int r, const int c)
{
    // edge cases
    if (r == 0 && c == 0) return 0 + proc->getVal(r, c);
    if (r == 0 && c  > 0) return times->getVal(r, c-1) + proc->getVal(r, c);
    if (r >  0 && c == 0) return times->getVal(r-1, c) + proc->getVal(r, c);


    // if the up job is slower than the left job, adjust all times in the column
    int up   = times->getVal(r-1, c);
    int left = times->getVal(r, c-1);

    if (up < left)
        incrimentPastJobs(times, left-up, r, c);

    // return the new value
    return times->getVal(r-1, c) + proc->getVal(r, c);
}

/**
 * @brief Incriments the completion time of previous jobs for the same machine
 * 
 * @param times     The matrix of completion times
 * @param diff      The value to incriment the prior jobs by
 * @param curRow    The current row, incriments all prior elements above this row
 * @param c         The current column
 */
void incrimentPastJobs(Matrix* times, int diff, const int curRow, const int c)
{
    for (int r = 0; r < curRow; ++r)
    {
        int newVal = times->getVal(r, c) + diff;
        times->setVal(newVal, r, c);
    }
}

