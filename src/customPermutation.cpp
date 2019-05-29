
#include <fstream>
#include <iostream>

#include "customPermutation.h"
#include "flowshop.h"

// using namespace std;

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
}

void initPerm(Permutation* perm, int* custPerm)
{
    // add all the elements from the array to the object in order
    for (int i = 0; i < perm->getSize(); ++i)
        perm->addElement(custPerm[i]);
}

/**
 * @brief Modifies a matrix so the columns are permutated relative
 *          to the original one.
 * 
 * @param jobs 
 * @param perm 
 * @param custPerm 
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
