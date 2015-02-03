#include <bits/stdc++.h>
#include <omp.h>
using namespace std;

#include "common.h"

void pSort(dataType *data, int ndata, SortType sorter)	{
	switch(sorter)	{
		case BEST:
		case MERGE:	mSort(data, ndata); break;

		default: fprintf(stderr, "Not implemented.");
	}
}