#include <stdio.h>
#include <omp.h>
using namespace std;

#include "sort.h"

void mSort(dataType *data, int n);
void qSort(dataType *data, int n);
void rSort(dataType *data, int n);

void pSort(dataType *data, int ndata, SortType sorter)	{
	switch(sorter)	{
		case BEST:
		case MERGE: mSort(data, ndata); break;
		case QUICK: qSort(data, ndata); break;
                case RADIX: rSort(data, ndata); break;

		default: fprintf(stderr, "Not implemented.");
	}
}
