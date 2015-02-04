#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <omp.h>

#include "sort.h"

const int ser_n = 1<<15;
const int radix = 1; // no of bits in radix

void rSort_helper(dataType *data, int n, int shift)    {
    // printf ("%d\n", n);
    if(n <= 1 || shift < 0)
        return;

    int r = (shift == 63) ? 1 : 0;
    int i = 0, j = n-1;

    dataType tmp;
    while(i <= j)   {
        tmp = data[i];
        data[i] = data[j];
        data[j] = tmp;

        while(i <= j &&
            (((long long)data[i].key >> shift) &1) == r)
                i++;
        while(i <= j &&
            (((long long)data[j].key >> shift) &1) != r)
                j--;
        // printf("%d %d\n", i, j);
    }
    if(i-j != 1)    {
        printf("ouch!\n");
        exit(-1);
    }

    #pragma omp task if(i > ser_n) firstprivate(data, j)
    rSort_helper(data, i, shift-1);

    #pragma omp task if(n-i > ser_n) firstprivate(data, j, i, n)
    rSort_helper(data+i, n-i, shift-1);
}

void rSort(dataType *data, int n)   {
    #pragma omp parallel firstprivate(data, n)
    {
        #pragma omp master
        {
            // printf("%d threads\n", omp_get_num_threads());
            rSort_helper(data, n, 63);
        }
    }
}
