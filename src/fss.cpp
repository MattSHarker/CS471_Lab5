
#include "fss.h"

/**
 * @brief The basic flowshop scheduling algorithm
 * 
 * @param jobs      The matrix of jobsess compTime
 * @param compTime  The resulting matrix of completion compTime
 * @param cols      How many columns to run through the algorithm
 * @return int      The result length the jobs will take
 */
int fss(Matrix* jobs, Matrix* compTime)
{
    // loop through the matrix and create the next values
    for (int c = 0; c < jobs->getCols(); ++c)
    {
        for (int r = 0; r < jobs->getRows(); ++r)
        {
            // calculate the new time and assign it
            int time = baseTimeFSS(compTime, r, c) + jobs->getVal(r, c);
            compTime->setVal(time, r, c);
        }
    }

    // return the final time
    return compTime->getVal(compTime->getRows()-1,  compTime->getCols()-1);
}




int fssPerm(Matrix* jobs, Matrix* compTime, Permutation* perm)
{
    // retreive the curent permutation size for easier reading
    int curSize = perm->getCurSize();

    // loop through the matrix and create the next values
    for (int c = 0; c < curSize; ++c)
    {
        for (int r = 0; r < jobs->getRows(); ++r)
        {
            // calculate the new time and assign it
            int time = baseTimeFSSPerm(jobs, compTime, perm, r, c);
            compTime->setVal(time, r, c);
        }
    }

    // return the final time
    return compTime->getVal(compTime->getRows()-1, curSize-1);
}




/**
 * @brief Calculates the base time for the next job completion time.
 *          Specialized for the basic FSS algorithm.
 * 
 * @param compTime  The matrix of completion compTime
 * @param r         The current row of the matrices
 * @param c         The current column of the matrices
 * @return int      The base time of the next completion time
 */
int baseTimeFSS(Matrix* compTime, const int r, const int c)
{
    // check for edge scenarios
    if (r == 0 && c == 0) return 0;
    if (r == 0 && c  > 0) return compTime->getVal(r, c-1);
    if (r >  0 && c == 0) return compTime->getVal(r-1, c);
    
    // if not an edge case, then return the max of the values to the left and above
    return max(compTime->getVal(r, c-1), compTime->getVal(r-1, c));
}


int baseTimeFSSPerm(Matrix* jobs, Matrix* compTime, Permutation* perm, const int r, const int c)
{
    // get the proper indices
    int colInd  = perm->getPerm(c);
    int colIndL = perm->getPerm(0);
    if (c > 0) colIndL = perm->getPerm(c-1); 

    // check for edge scenarios
    if (r == 0 && c == 0) return jobs->getVal(0, colInd);
    if (r >  0 && c == 0) return compTime->getVal(r-1, 0) + jobs->getVal(r, colInd);
    if (r == 0 && c  > 0) return compTime->getVal(0, c-1) + jobs->getVal(0, colInd);
    
    int left = compTime->getVal(r, c-1);
    int up   = compTime->getVal(r-1, c);

    // if not an edge case, then return the max of the values to the left and above
    return max(left, up) + jobs->getVal(r, colInd);
}
