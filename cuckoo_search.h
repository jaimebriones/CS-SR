#include "common.h"

#ifndef _CUCKOO_SEARCH_H
#define _CUCKOO_SEARCH_H


void CS(int[], int[], int, int, int, Nest *,double (*fobj)(int,double[],int));

double (*fobj)(double x[], int);

#endif
