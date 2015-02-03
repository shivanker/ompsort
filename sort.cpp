#include <stdio.h>
#include <omp.h>
using namespace std;

#include "sort.h"

void mSort(dataType *data, int n);

void pSort(dataType *data, int ndata, SortType sorter)	{
	switch(sorter)	{
		case BEST:
		case MERGE:	mSort(data, ndata); break;

		default: fprintf(stderr, "Not implemented.");
	}
}