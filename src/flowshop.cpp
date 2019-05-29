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
#include <random>
#include <thread>
#include <vector>

#include "customPermutation.h"
#include "flowshop.h"
#include "fss.h"
#include "fssb.h"
#include "fssnw.h"
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

    file.close();
}

void runFlowshop()
{
    // set up threadpool
    int numThreads =  thread::hardware_concurrency();
    ThreadPool tp(numThreads);
    vector<future<int>> futures;

    // create and initialize variables for the files to run
    int start;
    int end;
    initStartEnd(start, end);

    // for each algorithm
    for (int i = 0; i <= 2; ++i)
    {
        if      (i == 0) cout << "Starting FSS...\n";
        else if (i == 1) cout << "Starting FSSB...\n";
        else if (i == 2) cout << "Starting FSSNW...\n";

        // for each file
        for (int j = start; j <= end; ++j)
        {
            // add it to the pool
            futures.emplace_back(
                tp.enqueue(&flowshop, j, i)
            );
        }

        // join them
        for (int j = 0; j < futures.size(); ++j)
        {
            int val = futures[j].get();
            cout << "j: " << j+1 << endl;
        }

        // clear future list
        futures.clear();

        if      (i == 0) cout << "FSS has completed\n";
        else if (i == 1) cout << "FSSB has completed\n";
        else if (i == 2) cout << "FSSNW has completed\n";
    }
}

int flowshop(const int datafile, const int alg)
{
    // create a matrix for job times and one for completion times
    Matrix* jobs = new Matrix(datafile);
    Matrix* comp = new Matrix(jobs->getRows(), jobs->getCols());

    // create a memory object to record data
    Memory* mem = new Memory(jobs->getCols());

    // create a permutation object and initialize it
    Permutation* perm = new Permutation(jobs->getCols());
    initialize(jobs, perm); // adds the first element to the permutation

    // initialize randomization
    random_device rd;
    mt19937 mt(rd());
    uniform_real_distribution<double> distr(0, 1);


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
            // if they're the same randomly select one to keep
            else if (fit = curBest)
            {
                if (distr(mt) < 0.5)
                {
                    curBest = fit;
                    perm->setCurrentToBest();
                }
            }

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


void initStartEnd(int &start, int &end)
{
    // open file to get params
    string path = "parameters/parameters.txt";
    ifstream file(path);

    if (file.is_open())
    {
        // retreive the values
        file >> start;
        file >> end;
    }
    else
    {
        cout << "Parameter file could not be found, exiting program\n";
        exit(EXIT_FAILURE);
    }

    // close the file
    file.close();
}



