
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "Matrix.h"

using namespace std;


Matrix::Matrix(const int r, const int c)
{
    // assign the sizes
    rows = r;
    cols = c;

    // construct the matrix
    matrix = new int*[rows];
    for (int i = 0; i < rows; ++i)
        matrix[i] = new int[cols];

    // set default values to 0
    for (int i = 0; i < rows; ++i)
        for (int j = 0; j < cols; ++j)
            matrix[i][j] = 0;
    
    // construct jobCosts array
    jobCosts = new int[cols];
}


Matrix::Matrix(int filename)
{
    // finalize the pathname of the file
    string pathname = "DataFiles/" + to_string(filename) + ".txt";

    // string to hold the current line
    string val;

    // open the file
    ifstream file(pathname);
    if (file.is_open())
    {
        // first line contains rows and columns
        file >> rows;
        file >> cols;

        // construct the matrix
        matrix = new int*[rows];
        for (int i = 0; i < rows; ++i)
            matrix[i] = new int[cols];

        // read in and assign the values to the matrix
        for (int i = 0; i < rows; ++i)
        {
            // set the value of the elements in the row
            for (int j = 0; j < cols; ++j)
            {
                // get the value and assign it
                file >> val;
                matrix[i][j] = stoi(val);
            }
        }
    }
    else
    {
        // if the file could not be opened
        cout << "Matrix input file count not be found\n";
        exit(EXIT_FAILURE);
    }

    // close the file
    file.close();

    // setup the jobCosts array
    jobCosts = new int[cols];
    generateJobCosts();
}

Matrix::~Matrix()
{
    if (matrix != nullptr)
    {
        for (int i = 0; i < rows; ++i)
            delete[] matrix[i];
        delete[] matrix;
    }

    if (jobCosts != nullptr)
        delete[] jobCosts;
}


int Matrix::getRows()
{
    return rows;
}


int Matrix::getCols()
{
    return cols;
}




void Matrix::generateJobCosts()
{
    // for each job (column)
    for (int c = 0; c < cols; ++c)
    {
        // reset the value
        jobCosts[c] = 0;

        // store the sum of each machine (row) of the job (column)
        for (int r = 0; r < rows; ++r)
            jobCosts[c] += matrix[r][c];
    }
}

int Matrix::getJobCost(const int job)
{
    return jobCosts[job];
}

int* Matrix::getJobCosts()
{
    return jobCosts;
}


int Matrix::getVal(const int r, const int c)
{
    return matrix[r][c];
}

int Matrix::getFinalVal()
{
    return matrix[rows-1][cols-1];
}

void Matrix::setVal(int newVal, const int r, const int c)
{
    matrix[r][c] = newVal;
}

void Matrix::clearMatrix()
{
    for (int r = 0; r < rows; ++r)
        for (int c = 0; c < cols; ++c)
            matrix[r][c] = 0;
}

void Matrix::resize(const int newR, const int newC)
{
    // assign the size values
    rows = newR;
    cols = newC;

    // create a new temp array of the new size
    int** temp = new int*[rows];
    for (int i = 0; i < rows; ++i)
        temp[i] = new int[cols];
    
    // change the pointer of the original array to that of the new one
    matrix = temp;

    // delete the temp matrix
    for (int i = 0; i < rows; ++i)
        delete[] temp[i];
    delete[] temp;
}

/**
 * @brief Prints out the information of the Matrix object
 * 
 */
void Matrix::print()
{
    // print out row/col numbers
    cout << "Rows:    " << rows << "\n";
    cout << "Columns: " << cols << "\n";

    // print out the matrix
    for (int r = 0; r < rows; ++r)
    {
        for (int c = 0; c < cols; ++c)
            cout << matrix[r][c] << "\t";

        cout << "\n";
    }
}

void Matrix::printByFullPerm(Permutation* perm)
{
    // print out row/col numbers
    cout << "Rows:    " << rows << "\n";
    cout << "Columns: " << cols << "\n";

    // print out the matrix
    for (int r = 0; r < rows; ++r)
    {
        for (int c = 0; c < cols; ++c)
        {
            int index = perm->getJobOrder(c);
            cout << matrix[r][index] << '\t';

        }

        cout << '\n';
    }
    cout << '\n';

    for (int c = 0; c < cols; ++c)
    {
        cout << perm->getJobValue(c) << '\t';
    }
    cout << '\n';

    for (int c = 0; c < cols; ++c)
    {
        cout << perm->getJobOrder(c) << '\t';
    }
    cout << '\n';
}


void Matrix::printByCurrentPerm(Permutation* perm)
{
    // print out row/col numbers
    cout << "Rows:    " << rows << "\n";
    cout << "Columns: " << cols << "\n";

    // print out the matrix
    for (int r = 0; r < rows; ++r)
    {
        for (int c = 0; c < perm->getCurSize(); ++c)
        {
            int index = perm->getPerm(c);
            cout << matrix[r][index] << '\t';

        }

        cout << '\n';
    }
    cout << '\n';

    for (int c = 0; c < cols; ++c)
    {
        cout << perm->getJobValue(c) << '\t';
    }
    cout << '\n';

    for (int c = 0; c < cols; ++c)
    {
        cout << perm->getJobOrder(c) << '\t';
    }
    cout << '\n';
}






