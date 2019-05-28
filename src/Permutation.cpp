
#include <iostream>
#include <climits>

#include "Permutation.h"
// #include "Matrix.h"

using namespace std;

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

Permutation::~Permutation()
{
    delete[] perm;
}

int Permutation::getPos()
{
    return pos;
}


void Permutation::setPos(const int newPos)
{
    pos = newPos;
}

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

int Permutation::getSize()
{
    return size;
}


int Permutation::getCurSize()
{
    return curSize;
}

void Permutation::setCurSize(const int s)
{
    curSize = s;
}

void Permutation::incrimentCurSize()
{
    ++curSize;
}


int Permutation::getBestVal()
{
    return bestVal;
}

void Permutation::setBestVal(const int newVal)
{
    bestVal = newVal;
}


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

int Permutation::getPerm(const int elem)
{
    return perm[elem];
}

int* Permutation::getPerm()
{
    return perm;
}

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

void Permutation::printCurrentPermutation()
{
    for (int i = 0; i < curSize; ++i)
        cout << perm[i] << ' ';
    cout << '\n';
}


void Permutation::setCurrentToBest()
{
    // copy all the values from perm into best
    for (int i = 0; i < size; ++i)
        best[i] = perm[i];
}

void Permutation::setBestToCurrent()
{
    // copy all values from best into perm (or 0 for those that don't exist)
    for (int i = 0; i < curSize; ++i)
        perm[i] = best[i];
    
    // for the rest
    for (int i = curSize; i < size; ++i)
        perm[i] = 9;
}

int Permutation::getBest(const int elem)
{
    return best[elem];
}

int* Permutation::getBest()
{
    return best;
}





int Permutation::getJobOrder(const int elem)
{
    return allJobs[0][elem];
}

int Permutation::getJobValue(const int elem)
{
    return allJobs[1][elem];
}

int* Permutation::getJobOrder()
{
    return allJobs[0];
}

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


void Permutation::print()
{
    cout << "size: "    << size    << "\n";
    cout << "curSize: " << curSize << "\n";
    cout << "pos: "     << pos     << "\n";
    cout << "bestVal: " << bestVal << "\n";

    printAllJobs();

}



