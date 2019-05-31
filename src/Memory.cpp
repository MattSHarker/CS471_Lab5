/**
 * @file Memory.cpp
 * @author Matthew Harker
 * @brief A class that helps print out the results of each flowshop algorithm
 * @version 1.0
 * @date 2019-05-31
 * 
 * @copyright Copyright (c) 2019
 * 
 */
#include <iostream>
#include <fstream>
#include <sstream>

#include "flowshop.h"
#include "Matrix.h"
#include "Memory.h"
#include "Permutation.h"

/**
 * @brief Construct a new Memory:: Memory object
 * 
 */
Memory::Memory()
{
    funcCalls = 0;
}

/**
 * @brief Inciments the funcCalls variable by 1
 * 
 */
void Memory::incrFuncCalls()
{
    ++funcCalls;
}

/**
 * @brief Returns the value of funcCalls
 * 
 * @return int The number of function calls made
 */
int Memory::getFuncCalls()
{
    return funcCalls;
}

/**
 * @brief Starts a timer
 * 
 */
void Memory::startTimer()
{
    timer = clock();
}

/**
 * @brief Ends the timer and assigns the value to timeTaken
 * 
 */
void Memory::stopTimer()
{
    timer = clock() - timer;
    timeTaken = double(timer*1000)/CLOCKS_PER_SEC;
}

/**
 * @brief Controlss all the file writing functions
 * 
 * @param jobs      The matrix of job run times
 * @param comp      The matrix of job completion times
 * @param perm      The permutation object containing the best job sequence
 * @param alg       The algorithm that the system went through
 * @param datafile  Which datafile the job run time matrix used
 */
void Memory::writeAllData(Matrix* jobs, Matrix* comp, Permutation* perm, const int alg, const int datafile)
{
    writeRawData  (jobs, comp, perm, alg, datafile);
    writeGanttData(jobs, comp, perm, alg, datafile);
}

/**
 * @brief Writes the data in a format which is easier to read by humans
 * 
 * @param jobs      The matrix of job run times
 * @param comp      The matrix of job completion times
 * @param perm      The permutation object containing the best job sequence
 * @param alg       The algorithm that the system went through
 * @param datafile  Which datafile the job run time matrix used
 */
void Memory::writeRawData(Matrix* jobs, Matrix* comp, Permutation* perm, const int alg, const int datafile)
{
    // set the correct path to write the file to amd open it
    string pathname = "results/rawData/";
    if      (alg == 1) pathname += "FSS/fss-";
    else if (alg == 2) pathname += "FSSB/fssb-";
    else if (alg == 3) pathname += "FSSNW/fssnw-";
    pathname += to_string(datafile) += ".txt";
    ofstream txt(pathname);

    txt << "Dimensions (RxC): ";
    txt << jobs->getRows() << " " << jobs->getCols() << '\n';   // dimensions of the matrix
    txt << "Function calls: " << funcCalls << "\n";             // number of func calls
    txt << "Time taken: " << timeTaken << "\n\n";               // time taken (ms)

    // write the optimized fitness and the original fitness
    txt << "Optimized Cmax: " <<  perm->getBestVal() << '\n';
    txt << "Original Cmax: "; 
    Matrix* temp = new Matrix(jobs->getRows(), jobs->getCols());    // temp mtx to preserve data
    if      (alg == 1) txt << fss  (jobs, temp) << '\n';
    else if (alg == 2) txt << fssb (jobs, temp) << '\n';
    else if (alg == 3) txt << fssnw(jobs, temp) << '\n';
    delete temp;

    // write the permutation sequence
    txt << "\nPermutaion sequence:\n";
    txt << perm->getBest(0) + 1;
    for (int i = 1; i < perm->getSize(); ++i)
        txt << "," << perm->getBest(i) + 1;
    txt << '\n';

    // write the completion times
    txt << "\nCompletion times:\n";

    for (int r = 0; r < jobs->getRows(); ++r)
    {
        // write first value for no trailing commas
        txt << comp->getVal(r, 0);

        // write the rest of the values
        for (int c = 1; c < jobs->getCols(); ++c)
            txt << "," << comp->getVal(r, c);
        txt << '\n';
    }


    // write the start times (completion time minues)
    txt << "\nStart times:\n";

    for (int r = 0; r < jobs->getRows(); ++r)
    {
        // write first value for no trailing commas
        int ind = perm->getPerm(0);
        txt << comp->getVal(r, 0) - jobs->getVal(r, ind);

        // write the rest of the values
        for (int c = 1; c < jobs->getCols(); ++c)
        {
            ind = perm->getPerm(c);
            txt << "," << comp->getVal(r, c) - jobs->getVal(r, ind);
        }
        txt << '\n';
    }
}

/**
 * @brief Writes the data in a format that can be read by an R script to
 *          create Gantt charts of the data
 * 
 * @param jobs      The matrix of job run times
 * @param comp      The matrix of job completion times
 * @param perm      The permutation object containing the best job sequence
 * @param alg       The algorithm that the system went through
 * @param datafile  Which datafile the job run time matrix used
 */
void Memory::writeGanttData(Matrix* jobs, Matrix* comp, Permutation* perm, const int alg, const int datafile)
{
    // set the correct path to write the file to and open it
    string pathname = "results/ganttData/";
    if      (alg == 1) pathname += "FSS/fss_gantt-";
    else if (alg == 2) pathname += "FSSB/fssb_gantt-";
    else if (alg == 3) pathname += "FSSNW/fssnw_gantt-";
    pathname += to_string(datafile) += ".csv";
    ofstream csv(pathname);

    // write the header to the file
    csv << "Item,Machine,Job,Start,End" << '\n';

    // fill in the rest of the data
    int item = 0;
    for (int r = 0; r < jobs->getRows(); ++r)
    {
        for (int c = 0; c < jobs->getCols(); ++c)
        {
            int ind = perm->getPerm(c);
            csv << ++item << ",Machine " << r+1 << ",Job " << c+1 << ',';
            csv << comp->getVal(r, c) - jobs->getVal(r, ind);
            csv << ',' << comp->getVal(r, c) << '\n';
        }

    }
}
