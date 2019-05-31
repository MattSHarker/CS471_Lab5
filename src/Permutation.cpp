/**
 * @file Permutation.cpp
 * @author Matthew Harker
 * @brief A class that contains information to help with the NEH algorithm
 * @version 1.0
 * @date 2019-05-30
 * 
 * @copyright Copyright (c) 2019
 * 
 */
#include <climits>
#include <iostream>

#include "Permutation.h"

using namespace std;

/**
 * @brief Construct a new Permutation:: Permutation object
 * 
 * @param s the number of jobs in the system (size)
 */
Permutation::Permutation(const int s)
{
    size    = s;
    curSize = 0;
    pos     = 0;

    bestVal = INT_MAX;

    // set up perm and best arrays
    perm = new int[size];
    best = new int[size];

    // set up allJobs matrix 
    allJobs = new int*[2];
    for (int i = 0; i < 2; ++i)
        allJobs[i] = new int[size];
}

/**
 * @brief Destroy the Permutation:: Permutation object
 * 
 */
Permutation::~Permutation()
{
    delete[] perm;
}

/**
 * @brief Returns the value of pos
 * 
 * @return int The position of the most recently added element
 */
int Permutation::getPos()
{
    return pos;
}

/**
 * @brief Sets the value of pos
 * 
 * @param newPos The new position of the most recently added element
 */
void Permutation::setPos(const int newPos)
{
    pos = newPos;
}

/**
 * @brief Decriments the value of pos by 1
 * 
 */
void Permutation::nextPos()
{
    if (pos > 0)
        --pos;
    else
    {
        cout << "Decrimenting \"pos\" variable into negative values\n";
        exit(EXIT_FAILURE);
    }
}

/**
 * @brief Returns the full size of the permutation array
 * 
 * @return int The full size of the permutation array
 */
int Permutation::getSize()
{
    return size;
}

/**
 * @brief Returns the current size of the permutation array
 * 
 * @return int The current size of the permutation array
 */
int Permutation::getCurSize()
{
    return curSize;
}

/**
 * @brief Sets the current size of the permutation array
 * 
 * @param s The current size of the permutation array
 */
void Permutation::setCurSize(const int s)
{
    curSize = s;
}

/**
 * @brief Incriments the current size of the permutation array by 1
 * 
 */
void Permutation::incrimentCurSize()
{
    ++curSize;
}

/**
 * @brief Returns the value of bestVal
 * 
 * @return int the value of the current best permutation
 */
int Permutation::getBestVal()
{
    return bestVal;
}

/**
 * @brief Sets the value of bestVal
 * 
 * @param newVal The new value of the newest best found permutation
 */
void Permutation::setBestVal(const int newVal)
{
    bestVal = newVal;
}

/**
 * @brief Adds an element to the perm matrix
 * 
 * @param newElem The next job to add to the sequence
 */
void Permutation::addElement(const int newElem)
{
    // only do it if it won't go out of range
    if (curSize < size)
    {
        perm[curSize] = newElem;
        pos = curSize;
        ++curSize;
    }
    else
    {
        cout << "Cannot add element, permutation array is full\n";
        exit(EXIT_FAILURE);
    }
}

/**
 * @brief The next permutation of the NEH algorithm
 * 
 */
void Permutation::nextPermutation()
{
    // only do it if it won't go out of range
    if (pos > 0)
    {
        swap(perm[pos], perm[pos-1]);
        --pos;
    }
    else
    {
        cout << "Cannot swap elements that do not exist\n";
        exit(EXIT_FAILURE);
    }
}

/**
 * @brief Returns the job in the specified element of the perm array
 * 
 * @param elem The element to get the job index of
 * @return int An index of the job run time matrix
 */
int Permutation::getPerm(const int elem)
{
    return perm[elem];
}

/**
 * @brief Returns the full perm array
 * 
 * @return int* The current best permutation
 */
int* Permutation::getPerm()
{
    return perm;
}

/**
 * @brief Resizes the perm array
 * 
 * @param newSize The new size of the perm array
 */
void Permutation::resize(const int newSize)
{
    // change the value of size
    size = newSize;

    // create a new array of the new size, and change perm's pointer to that
    int* temp = new int[size];
    perm = temp;

    // delete the temp array
    delete[] temp;
}

/**
 * @brief Prints the current permutation
 * 
 */
void Permutation::printCurrentPermutation()
{
    for (int i = 0; i < curSize; ++i)
        cout << perm[i] << ' ';
    cout << '\n';
}

/**
 * @brief Sets the values of best to those of perm
 * 
 */
void Permutation::setCurrentToBest()
{
    // copy all the values from perm into best
    for (int i = 0; i < size; ++i)
        best[i] = perm[i];
}

/**
 * @brief Sets the values of perm to those of best
 * 
 */
void Permutation::setBestToCurrent()
{
    // copy all values from best into perm (or 0 for those that don't exist)
    for (int i = 0; i < curSize; ++i)
        perm[i] = best[i];
    
    // for the rest
    for (int i = curSize; i < size; ++i)
        perm[i] = 9;
}

/**
 * @brief Returns an element from the best array
 * 
 * @param elem The element to return
 * @return int An index of the job run time matrix
 */
int Permutation::getBest(const int elem)
{
    return best[elem];
}

/**
 * @brief Returns the full best array
 * 
 * @return int* The full best array
 */
int* Permutation::getBest()
{
    return best;
}

/**
 * @brief Returns an index from the allJobs matrix
 * 
 * @param elem The element containing an index of the job run time array
 * @return int The index of the job run time array
 */
int Permutation::getJobOrder(const int elem)
{
    return allJobs[0][elem];
}

/**
 * @brief Returns the total job cost from the allJobs matrix
 * 
 * @param elem The element containg the total job cost
 * @return int The total job cost
 */
int Permutation::getJobValue(const int elem)
{
    return allJobs[1][elem];
}

/**
 * @brief Returns the full array of job indices
 * 
 * @return int* The array of job indices
 */
int* Permutation::getJobOrder()
{
    return allJobs[0];
}

/**
 * @brief Sets the total job cost values of the allJ
 * 
 * @param costs 
 */
void Permutation::setAllJobs(const int* costs)
{
    for (int i = 0; i < size; ++i)
    {
        allJobs[0][i] = i;
        allJobs[1][i] = costs[i];
    }
}

/**
 * @brief Sorts the allJobs matrix based on the total cost of each job.
 *          Utilizes insertion sort.
 * 
 */
void Permutation::sortAllJobs()
{
    for (int i = 1; i < size; i++)
    {
        // store the current index and value
        int curInd = allJobs[0][i];
        int curVal = allJobs[1][i];

        int j = i - 1;

        while (j >= 0 && allJobs[1][j] < curVal)
        {
            allJobs[0][j+1] = allJobs[0][j];
            allJobs[1][j+1] = allJobs[1][j];
            --j;
        }
        
        // set the stored index and value
        allJobs[0][j+1] = curInd;
        allJobs[1][j+1] = curVal;
    }
}

/**
 * @brief Prints the values of allJobs
 * 
 */
void Permutation::printAllJobs()
{
    // print the job indices
    for (int i = 0; i < size; ++i)
        cout << allJobs[0][i] << "\t";
    cout << "\n";

    // print the job costs
    for (int i = 0; i < size; ++i)
        cout << allJobs[1][i] << '\t';
    cout << '\n';
}

/**
 * @brief Prints the variables of the object
 * 
 */
void Permutation::print()
{
    cout << "size: "    << size    << "\n";
    cout << "curSize: " << curSize << "\n";
    cout << "pos: "     << pos     << "\n";
    cout << "bestVal: " << bestVal << "\n";

    printAllJobs();

}
