/**
 * @file flowshop.cpp
 * @author Matthew Harker
 * @brief Contains three flow shop algorithms: FSS, FSSB, and FSSNW.
            Takes in a matrix of jobsess compTime, and creates another
            matrix of completion compTime for each job/machine.

 * @version 1.0
 * @date 2019-05-31
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#include <climits>
#include <fstream>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

#include "customPermutation.h"
#include "flowshop.h"
#include "ThreadPool.h"

using namespace std;

void run()
{
    int custPerm;

    // read in wether to use a customer permutation or run all
    string path = "parameters/custPerm.txt";
    ifstream file(path);
    if (file.is_open())
    {
        file >> custPerm;
        file.close();

        if (custPerm == 1)
            customPermutation();
        else
            runFlowshop();
    }
    else
    {
        cout << "Custom permutation file not found, continuing with NEH algorithm\n";
        runFlowshop();
    }
}

void runFlowshop()
{
    // set up threadpool
    int numThreads =  thread::hardware_concurrency();
    ThreadPool tp(numThreads);
    vector<future<int>> futures;
    
    // for each algorithm
    for (int i = 0; i <= 0; ++i)
    {
        Memory* mem = new Memory();

        if      (i == 0) cout << "Starting FSS...\n";
        else if (i == 1) cout << "Starting FSSB...\n";
        else if (i == 2) cout << "Starting FSSNW...\n";

        // for each file
        for (int j = 0; j <= 10; ++j)
        {
            // add it to the pool
            futures.emplace_back(
                tp.enqueue(&flowshop, mem, j, i)
            );
        }

        // join them
        for (int j = 0; j < futures.size(); ++j)
            int val = futures[j].get();

        // clear future list
        futures.clear();

        if      (i == 0) cout << "FSS has completed\n";
        else if (i == 1) cout << "FSSB has completed\n";
        else if (i == 2) cout << "FSSNW has completed\n";

        delete mem;
    }
}

int flowshop(Memory* mem, const int datafile, const int alg)
{
    // create a matrix for job times and one for completion times
    Matrix* jobs = new Matrix(datafile);
    Matrix* comp = new Matrix(jobs->getRows(), jobs->getCols());

    // create a permutation object and initialize it
    Permutation* perm = new Permutation(jobs->getCols());
    initialize(jobs, perm); // adds the first element to the permutation

    // for every other element to be permutated
    for (int j = 1; j < jobs->getCols(); ++j)
    {
        int curBest = INT_MAX; // holds best val of the iteration

        // add the next element
        perm->addElement(perm->getJobOrder(j));

        // go through each NEH permutation of the array and keep the best
        for (int k = 0; k <= perm->getCurSize(); ++k)
        {
            // get the fitness of the permutation
            comp->clearMatrix();
            int fit = fssTypePerm(jobs, comp, perm, alg);
            mem->incrFuncCalls();

            // if it's better (lower) store the fitness and the permutaion
            if (fit < curBest)
            {
                curBest = fit;              // save the fitness value
                perm->setCurrentToBest();   // save the current permutation
            }

            ///////////////////////////////////////////
            // if it's the same randomly choose one
            //
            //
            //
            //
            ///////////////////////////////////////////




            // if it's not the last permutation get the next one
            if (perm->getPos() > 0) perm->nextPermutation();
        }

        // save the best fitness and permutation
        perm->setBestVal(curBest);
        perm->setBestToCurrent();

        // rerun the flowshop to store the correct data to $comp
        comp->clearMatrix();
    }

    // rerun to have the correct info in comp
    int temp = fssTypePerm(jobs, comp, perm, alg);

    mem->writeCSV(jobs, comp, perm, alg, datafile); // write data to a file

    // delete the objects
    delete jobs;
    delete comp;
    delete perm;

    return 0;
}

int fssType(Matrix* jobs, Matrix* comp, const int alg)
{
    switch(alg)
    {
        case 0: return fss(jobs, comp);
        case 1: return fssb(jobs, comp);
        case 2: return fssnw(jobs, comp);
    }
}

int fssTypePerm(Matrix* jobs, Matrix* comp, Permutation* perm, const int alg)
{
    switch(alg)
    {
        case 0: return fssPerm(jobs, comp, perm);
        case 1: return fssbPerm(jobs, comp, perm);
        case 2: return fssnwPerm(jobs, comp, perm);
    }
}


void initialize(Matrix* jobs, Permutation* perm)
{
    // create the sorted job matrix in perm
    jobs->generateJobCosts();
    perm->setAllJobs(jobs->getJobCosts());
    perm->sortAllJobs();
    perm->addElement(perm->getJobOrder(0));
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
