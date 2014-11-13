#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "RngStream.h"
#include "cuckoo_search.h"
#include "common.h"
#include "StRnk.h"


void init(int nnest, int dim, int lb[], int ub[], Nest *Nests, RngStream g1){
	int i;
	for(i=0;i<nnest;i++){
		int j;
		for(j=0;j<dim;j++){
			Nests[i].pos[j] = lb[j]+(ub[j]-lb[j])*RngStream_RandU01(g1);
		}
		Nests[i].fx = INFINITY;
	}

}

void show_nest(int nnest, int dim, Nest *Nests, Nest *new_Nest, Nest *best, double fxmin){

	printf("%f\n", fxmin);
}

void simplebounds(int nnest, int dim, Nest *new_Nest, int lb[], int ub[]){
	int i;
	for (i = 0; i < dim; i++)
	{	
		double avg = 0;
		int j;
		for (j = 0; j < nnest; j++){
			avg = avg + new_Nest[j].pos[i];
		}
		avg = avg/nnest;

		for (j = 0; j < nnest; j++){
			if(new_Nest[j].pos[i] > ub[j] || new_Nest[j].pos[i] < lb[j]){
				new_Nest[j].pos[i] = avg;
				
			}
		}
	}

}

void get_best_nest(int nnest, int dim, Nest *best, Nest *new_Nest, Nest *Nests,double (*fobj)(int, double x[], int)){
	int i, auxIndex;
	double auxMin=INFINITY;
	for (i = 0; i < nnest; i++)
	{
		
		double min = fobj(nnest, new_Nest[i].pos, dim);
	
		if(min < Nests[i].fx){
			Nests[i].fx = min;
			memcpy(Nests[i].pos, new_Nest[i].pos, sizeof(Nests[i].pos));
		}
		if(Nests[i].fx < auxMin){
				auxMin = Nests[i].fx;
				auxIndex = i;
		}
	}
	memcpy(best->pos, Nests[auxIndex].pos, sizeof(best->pos));
	best->fx = auxMin;

}

void empty_nests(int nnest, int dim, int lb[], int ub[], Nest *new_Nest, Nest *Nests, double pa, RngStream g1){

	int i;
	for (i = 0; i < nnest; i++){
		int j;
		for (j = 0; j < dim; j++){
			double num = RngStream_RandU01(g1);
			if(num>pa){
				int rindex = rand() % nnest + 1;
				double stepsize = num * Nests[rindex].pos[j] - Nests[rindex].pos[j];
				new_Nest[i].pos[j] = Nests[i].pos[j] + stepsize;
			}
		}
	}

	simplebounds(nnest, dim, new_Nest, lb, ub);
	
}

void get_cuckoos(int nnest, int lb[], int ub[], int dim, Nest *best, Nest *new_Nest, Nest *Nests, RngStream g1){

	double beta = 3/2;
	double sigma = pow((tgamma(1.0+beta) * sin(M_PI*beta/2.0) / ((tgamma(1.0+beta)/2.0) * beta * (pow(2.0,((beta-1.0)/2.0))))),(1.0/beta));

	int i;
	for (i = 0; i < nnest; i++){
		int j;
		for (j = 0; j < dim; j++){
			double step = (RngStream_RandU01(g1)*sigma) / pow(fabs(RngStream_RandU01(g1)),(1/beta));
			double stepsize = 0.01 * step * (best->pos[j]);

			new_Nest[i].pos[j] = Nests[i].pos[j] + stepsize * RngStream_RandU01(g1);
		}
		
	}
	simplebounds(nnest, dim, new_Nest, lb, ub);

}

void CS(int lb[], int ub[], int maxI, int nnest, int dim, Nest *Nests, double (*fobj)(int, double x[], int)){
	srand(time(0));

	unsigned long germe[6] = { rand(), rand(), rand(), rand(), rand(), rand() };
    RngStream_SetPackageSeed (germe);
    RngStream g1 = RngStream_CreateStream ("Laplace");

	double pa = 0.25;

	Nest new_Nest[nnest], best;

	init(nnest,dim,lb,ub,Nests,g1);
	memcpy(new_Nest, Nests, sizeof(new_Nest));
	get_best_nest(nnest,dim, &best, new_Nest, Nests,fobj);
	double fxmin = best.fx;
	
	int t;
	for (t = 0; t < 1; t++){

		get_cuckoos(nnest,lb, ub, dim, &best, new_Nest, Nests,g1);
		get_best_nest(nnest,dim, &best, new_Nest, Nests, fobj);
		empty_nests(nnest,dim, lb, ub, new_Nest, Nests,pa,g1);
		get_best_nest(nnest,dim, &best, new_Nest, Nests, fobj);

		if(best.fx < fxmin){
			fxmin = best.fx;
		}

		stRank(Nests, nnest, dim, g1);
	}
	
	show_nest(nnest,dim, Nests, new_Nest, &best, fxmin);
}
