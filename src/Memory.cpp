
#include <fstream>
#include <sstream>

#include "flowshop.h"
#include "Matrix.h"
#include "Memory.h"
#include "Permutation.h"


Memory::Memory()
{
    funcCalls = 0;
    bestPermutation = new int[200];
}

Memory::Memory(const int s)
{
    funcCalls = 0;

    size = s;
    bestPermutation = new int[size];
}

Memory::~Memory()
{
    if (bestPermutation != nullptr)
        delete[] bestPermutation;
}


void Memory::incrFuncCalls()
{
    ++funcCalls;
}

int Memory::getFuncCalls()
{
    return funcCalls;
}

int Memory::getOriginalFit()
{
    return originalFit;
}

void Memory::setOriginalFit(const int newFit)
{
    originalFit = newFit;
}

int Memory::getBestFit()
{
    return bestFit;
}

void Memory::setBestFit(const int newFit)
{
    bestFit = newFit;
}


int Memory::getSize()
{
    return size;
}


int Memory::getBestPermutation(const int elem)
{
    return bestPermutation[elem];
}

int* Memory::getBestPermutation()
{
    return bestPermutation;
}

void Memory::setBestPermutation(int newVal, const int elem)
{
    bestPermutation[elem] = newVal;
}

void Memory::setBestPermutation(int* newPerm)
{
    for (int i = 0; i < size; ++i)
        bestPermutation[i] = newPerm[i];
}

void Memory::resizeBestPermutation(const int s)
{
    // create temp array
    int* temp = new int[s];

    // change the pointer of bestPerm to the temp's
    bestPermutation = temp;

    // change the value of size
    size = s;

    // destroy the temp array
    delete[] temp;
}


void Memory::recordData(Matrix* jobs, Matrix* comp, Permutation* perm, const int alg)
{
    // record the original fitness
    if (alg == 0) originalFit == fss  (jobs, comp);
    else if (alg == 1) originalFit == fssb (jobs, comp);
    else if (alg == 2) originalFit == fssnw(jobs, comp);

    // record the permutation
    for (int i = 0; i < perm->getSize(); ++i)
        bestPermutation[i] = perm->getBest(i);
}

void Memory::writeCSV(Matrix* jobs, Matrix* comp, Permutation* perm, const int alg, const int datafile)
{
    // set the correct path to write the file to
    string pathname = "results/";
    if      (alg == 0) pathname += "FSS/fss-";
    else if (alg == 1) pathname += "FSSB/fssb-";
    else if (alg == 2) pathname += "FSSNW/fssnw-";

    // add the number of the file and the extension to the path name and open it
    pathname += to_string(datafile) += ".csv";
    ofstream csv(pathname);

    csv << jobs->getRows() << " " << jobs->getCols() << '\n';   // dimensions of the matrix
    csv << funcCalls << "\n\n";                                   // number of func calls

    // write the optimized fitness and the original fitness
    csv << perm->getBestVal() << '\n';    
    Matrix* temp = new Matrix(jobs->getRows(), jobs->getCols());    // temp mtx to preserve data
    if      (alg == 0) csv << fss  (jobs, temp) << '\n';
    else if (alg == 1) csv << fssb (jobs, temp) << '\n';
    else if (alg == 2) csv << fssnw(jobs, temp) << '\n';
    delete temp;

    // write the permutation sequence
    csv << "\nPermutaion sequence:\n";
    csv << perm->getBest(0) + 1;
    for (int i = 1; i < perm->getSize(); ++i)
        csv << "," << perm->getBest(i) + 1;
    csv << '\n';

    // write the completion times
    csv << "\nCompletion times:\n";

    for (int r = 0; r < jobs->getRows(); ++r)
    {
        // write first value for no trailing commas
        csv << comp->getVal(r, 0);

        // write the rest of the values
        for (int c = 1; c < jobs->getCols(); ++c)
            csv << "," << comp->getVal(r, c);
        csv << '\n';
    }


    // write the start times (completion time minues)
    csv << "\nStart times:\n";

    for (int r = 0; r < jobs->getRows(); ++r)
    {
        // write first value for no trailing commas
        int ind = perm->getPerm(0);
        csv << comp->getVal(r, 0) - jobs->getVal(r, ind);

        // write the rest of the values
        for (int c = 1; c < jobs->getCols(); ++c)
        {
            ind = perm->getPerm(c);
            csv << "," << comp->getVal(r, c) - jobs->getVal(r, ind);
        }
        csv << '\n';
    }
}

