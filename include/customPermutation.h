
#ifndef CUST_PERM_H
#define CUST_PERM_H

#include <string>

#include "Matrix.h"

using namespace std;

void customPermutation();
void runCustPerm(Matrix* jobs, const int alg);

void initVars(string &filename, int &alg, int &size, int* &custPerm);
void initPerm(Permutation* perm, int* custPerm);
void initPermMatrix(Matrix* jobs, Matrix* perm, int* custPerm);

#endif
