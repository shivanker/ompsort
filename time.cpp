#include <bits/stdc++.h>
#include <omp.h>
using namespace std;

#include "common.h"

int main()  {
    const int n = ((1<<27) * 3)/4;
    int i;
    dataType *data = new dataType[n];
    
    #pragma omp parallel firstprivate(data)
    {
        srand(time(NULL) ^ omp_get_thread_num());
        srand(rand()     ^ omp_get_thread_num());
        #pragma omp for
        for(i = 0; i < n; ++i)  {
            data[i].key = (long long *)randull();
        }
    }

    printf("Beginning sorting..\n");
    
    double start, end;
    start = omp_get_wtime();
    pSort(data, n);
    end = omp_get_wtime();
    for(i = 0; i < n-1; ++i)
        if((long long)data[i].key > (long long)data[i+1].key) {
            printf("Sort failed!\n");
            return -1;
        }

    printf("Output looks sorted.\n");
    printf("Time taken: %.2fs\n", end-start);
    
    return 0;
}
