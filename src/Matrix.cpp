/**
 * @file Matrix.cpp
 * @author Matthew Harker
 * @brief Contains information to easier use 2D matrices.
 * @version 2.0
 * @date 2019-05-31
 * 
 * @copyright Copyright (c) 2019
 * 
 */
#include <fstream>
#include <iostream>
#include <string>

#include "Matrix.h"

using namespace std;

/**
 * @brief Construct a new Matrix:: Matrix object
 * 
 * @param r How many rows the matrix should have
 * @param c How many columns the matrix should have
 */
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

/**
 * @brief Construct a new Matrix:: Matrix object
 * 
 * @param filename The name of the file to read information from
 */
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

/**
 * @brief Construct a new Matrix:: Matrix object
 * 
 * @param filename The name of the file to read in from
 */
Matrix::Matrix(string filename)
{
    // finalize the pathname of the file
    string pathname = "DataFiles/" + filename;

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

/**
 * @brief Destroy the Matrix:: Matrix object
 * 
 */
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

/**
 * @brief Returns the number of rows
 * 
 * @return int The number of rows
 */
int Matrix::getRows()
{
    return rows;
}

/**
 * @brief Returns the number of columns
 * 
 * @return int The number of columns
 */
int Matrix::getCols()
{
    return cols;
}

/**
 * @brief Generates the total run time for each job.
 * 
 */
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

/**
 * @brief Returns the total job cost for one column
 * 
 * @param job   Which column to get the total job cost of
 * @return int  The total job cost
 */
int Matrix::getJobCost(const int job)
{
    return jobCosts[job];
}

/**
 * @brief Returns the full jobCosts array
 * 
 * @return int* The jobCosts array
 */
int* Matrix::getJobCosts()
{
    return jobCosts;
}

/**
 * @brief Returns the value of a specified element in the matrix
 * 
 * @param r     The row of the element
 * @param c     The column of the element
 * @return int  The value of the element
 */
int Matrix::getVal(const int r, const int c)
{
    return matrix[r][c];
}

/**
 * @brief Returns the bottom right element of the matrix
 * 
 * @return int 
 */
int Matrix::getFinalVal()
{
    return matrix[rows-1][cols-1];
}

/**
 * @brief Sets the value of a specified element in the matrix
 * 
 * @param newVal The new value of the element
 * @param r The row the element is in
 * @param c The column the element is in
 */
void Matrix::setVal(int newVal, const int r, const int c)
{
    matrix[r][c] = newVal;
}

/**
 * @brief Sets all of the values in the matrix to 0
 * 
 */
void Matrix::clearMatrix()
{
    for (int r = 0; r < rows; ++r)
        for (int c = 0; c < cols; ++c)
            matrix[r][c] = 0;
}

/**
 * @brief Resizes the matrix. Removes all data in the matrix
 * 
 * @param newR The new amount of rows
 * @param newC The new amount of columns
 */
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
 * @brief Prints out some information of the Matrix object
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
