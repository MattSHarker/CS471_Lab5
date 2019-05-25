
#include <iostream>

#include "Matrix.h"
#include "flowshop.h"

int main()
{
    Matrix* jobs  = new Matrix("1.txt");
    Matrix* compTimes = new Matrix(jobs->getRows(), jobs->getCols());

    int finalVal = fssnw(jobs, compTimes);

    cout << "final val: " << finalVal << endl;
    compTimes->print();

    jobs->


    delete jobs;
    delete compTimes;

    return 0;
}
