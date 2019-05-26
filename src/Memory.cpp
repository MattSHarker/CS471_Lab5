
#include "Memory.h"


Memory::Memory()
{
    funcCalls = 0;
}

Memory::Memory(const int s)
{
    funcCalls = 0;

    size = s
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

