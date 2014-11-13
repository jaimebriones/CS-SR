#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "StRnk.h"
#include "RngStream.h"
#include "ConsFunctions.h"
#include "common.h"

void stRank(Nest *nests, int nnest, int dim, RngStream g1){

	double Pf = 0.45;

	int i = 0;
	for (i = 0; i < nnest; ++i){
		int band = 0;
		int j = 0;

		double hlp[dim];

		for (j = 0; j < nnest; ++j){
			double u = RngStream_RandU01(g1);

			if (getPhi(nests[j].pos)==getPhi(nests[j+1].pos) || u < Pf){

				if (nests[j].fx > nests[j+1].fx){
					memcpy(hlp, nests[j].pos, sizeof(hlp));
					memcpy(nests[j].pos, nests[j+1].pos, sizeof(nests[j].pos));
					memcpy(nests[j+1].pos, hlp, sizeof(nests[j+1].pos));
					band = 1;
				}

			}
			else{

				if (nests[j].fx > nests[j+1].fx){
					memcpy(hlp, nests[j].pos, sizeof(hlp));
					memcpy(nests[j].pos, nests[j+1].pos, sizeof(nests[j].pos));
					memcpy(nests[j+1].pos, hlp, sizeof(nests[j+1].pos));
					band = 1;
				}

			}
		}

		if (band == 1){
			break;
		}
		
	}
}
