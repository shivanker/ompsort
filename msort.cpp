#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <omp.h>

#include "common.h"

const int ser_n = 1<<18;
dataType *orig_data;

void merge(int *data, int n1, int n2, int *res) {
    // printf("Thread %d doing merge.\n", omp_get_thread_num());
    int i = 0, j = n1, k = 0;

    while(i < n1 && j < n2)
        if((long long)orig_data[data[i]].key < (long long)orig_data[data[j]].key)
            res[k++] = data[i++];
        else
            res[k++] = data[j++];
    
    while(i < n1)
        res[k++] = data[i++];
    while(j < n2)
        res[k++] = data[j++];
}

void mSort_helper(int *data, int n, int *res)   {
    // printf("Thread %d\n", omp_get_thread_num());
    if(n == 1)  {
        res[0] = data[0];
        return;
    }
    if(n == 0) {
        return;
    }
    if(n < 0)   {
        #ifdef DEBUG
            printf("n < 0 in mSort_helper.\n");
        #endif
        exit(1);
    }
    
    #pragma omp task if(n > ser_n) firstprivate(res, n, data) untied
    mSort_helper(res, n/2, data);
 
    #pragma omp task if(n > ser_n) firstprivate(res, n, data) untied
    mSort_helper(res+n/2, n-n/2, data+n/2);

    #pragma omp taskwait
    merge(data, n/2, n, res);

}

void mSort(dataType *data, int n)    {
    int *res1 = new int[n];
    int *res2 = new int[n];
    orig_data = data;

    #pragma omp parallel firstprivate(res1, res2, n, data, orig_data)
    {
        int i;
        #pragma omp for
        for (i = 0; i < n; ++i)
        {
            res1[i] = i;
            res2[i] = i;
        }
        #pragma omp barrier
        #pragma omp master// implicit nowait
        {
            Dprintf("%d threads\n", omp_get_num_threads());
            mSort_helper(res2, n, res1);
        }
    }
    apply_perm(data, res1, n);

    delete [] res1;
    delete [] res2;
}
