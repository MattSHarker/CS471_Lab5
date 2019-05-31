/**
 * @file customPermutation.cpp
 * @author Matthew Harker
 * @brief 
 * @version 1.0
 * @date 2019-05-31
 * 
 * @copyright Copyright (c) 2019
 * 
 */
#include <fstream>
#include <iostream>

#include "customPermutation.h"
#include "flowshop.h"

/**
 * @brief The overall class that runs the ability to get the
 *          makespan of a system using a specified job sequence.
 * 
 */
void customPermutation()
{
    // vars for the permutation
    string filename;
    int alg, size;
    int* custPerm;

    // initialize variables then the permutation
    initVars(filename, alg, size, custPerm);

    // create matrix objects to store job times and a permutated version
    Matrix* jobs     = new Matrix(filename);
    Matrix* permJobs = new Matrix(jobs->getRows(), jobs->getCols());
    initPermMatrix(jobs, permJobs, custPerm);

    // run the flowshop algorithm
    runCustPerm(permJobs, alg);

    // destroy all of the objects
    delete jobs;
    delete permJobs;
    delete[] custPerm;
}

/**
 * @brief Takes in a job time matrix and calculates the makespan
 *          based on a user input job sequence
 * 
 * @param jobs  The matrix containing job run times
 * @param alg   Which algorithm to put the system though
 */
void runCustPerm(Matrix* jobs, const int alg)
{
    // create another matrix object to hold final times
    Matrix* comp = new Matrix(jobs->getRows(), jobs->getCols());

    // get the cmax of the custom permutation
    int cmax = fssType(jobs, comp, alg);

    cout << "The Cmax of the custom permutation using ";
    if      (alg == 0) cout << "FSS ";
    else if (alg == 1) cout << "FSSB ";
    else if (alg == 2) cout << "FSSB ";
    cout << "is: " << cmax << "\n";

    delete comp;
}

/**
 * @brief Initializes variables based on parameters in the custPerm.txt
 *          file.
 * 
 * @param filename  The name of the file to read in
 * @param alg       Which algorithm to put the matrix through
 * @param size      How many jobs are in the system
 * @param custPerm  The permutation of job orders
 */
void initVars(string &filename, int &alg, int &size, int* &custPerm)
{
    // open file to read in parameters
    string path = "parameters/custPerm.txt";
    ifstream file(path);
    
    if (file.is_open())
    {
        // retreive the variables
        string  skip;   // to ignore the first line
        file >> skip;   
        file >> filename;
        file >> alg;
        file >> size;
        
        // initialize the array and set it
        custPerm = new int[size];
        for (int i = 0; i < size; ++i)
            file >> custPerm[i];
        
        // close the file
        file.close();
    }
    else
    {
        cout << "Error reading custom permutation file, exiting program\n";
        exit(EXIT_FAILURE);
    }

    file.close();
}


/**
 * @brief Modifies a matrix so the columns are permutated relative
 *          to the original one.
 * 
 * @param jobs      The matrix containing the original job run times
 * @param perm      A matrix to hold the permutated job sequence
 * @param custPerm  An array that contains the sequence of jobs
 */
void initPermMatrix(Matrix* jobs, Matrix* perm, int* custPerm)
{
    for (int r = 0; r < jobs->getRows(); ++r)
    {
        for (int c = 0; c < jobs->getCols(); ++c)
        {
            int val = jobs->getVal(r, custPerm[c]-1);
            perm->setVal(val, r, c);
        }
    }
}
