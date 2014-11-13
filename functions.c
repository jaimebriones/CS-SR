#include <stdio.h>
#include <math.h>

#include "cuckoo_search.h"

#define lb -10
#define ub 10

double function(double x[], int dim){
	int i;
	double sum=0;
	for(i=0; i<dim; i++){
		sum = sum + pow(x[i],2);
	}

	return sum;
}

int main(){

	

	Nests Nests[nnest];
	double (*fObj)(double[],int);
	fObj = function;
	printf("funcion \n");
	CS(lb,ub,d,Nests,fObj);

	return 0;
}
