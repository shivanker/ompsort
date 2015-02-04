#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <omp.h>

#include "sort.h"

const int ser_n = 1<<13;

void qSort_helper(dataType *data, int n)    {
    // printf ("%d\n", n);
    if(n <= 1)
        return;

    int i = 1, j = n-1;
    long long pivot = (long long)data[0].key;
    dataType tmp;
    while(i <= j)   {
        tmp = data[i];
        data[i] = data[j];
        data[j] = tmp;

        while(i <= j &&
            (long long)data[i].key <= pivot)
                i++;
        while(i <= j &&
            pivot < (long long)data[j].key)
                j--;
        // printf("%d %d\n", i, j);
    }
    if(i-j != 1)    {
        printf("ouch!\n");
        exit(-1);
    }
    tmp = data[0];
    data[0] = data[j];
    data[j] = tmp;

    #pragma omp task if(j > ser_n) firstprivate(data, j)
    qSort_helper(data, j);

    #pragma omp task if(n-i > ser_n) firstprivate(data, j, i, n)
    qSort_helper(data+i, n-i);
}

void qSort(dataType *data, int n)   {
    #pragma omp parallel firstprivate(data, n)
    {
        #pragma omp master
        {
            // printf("%d threads\n", omp_get_num_threads());
            qSort_helper(data, n);
        }
    }
}
