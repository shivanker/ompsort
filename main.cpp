#include <bits/stdc++.h>
#include <omp.h>
using namespace std;

#include "common.h"

unsigned long long randull()   {
    return ((unsigned long long)rand() << (sizeof(int) * 8)) | rand();
}

int main()  {
    const int n = 10;
    int i, j;
    dataType *data = new dataType[n];

    #ifdef DEBUG
    for(int k = 0; k < 15; ++k)
    #endif
    {
        #pragma omp parallel firstprivate(data, j)
        {
            srand(time(NULL) ^ omp_get_thread_num());
            srand(rand()     ^ omp_get_thread_num());
            #pragma omp for
            for(i = 0; i < n; ++i)  {
                data[i].key = (long long *)randull();
            }
        }

        #ifdef DEBUG
        set<long long> ints;
        for(i = 0; i < n; ++i)
            ints.insert((long long)data[i].key);
        #endif
    
        
        NDprintf("Beginning sorting..\n");
        
        double start, end;
        start = omp_get_wtime();
        pSort(data, n);
        end = omp_get_wtime();
        for(i = 0; i < n-1; ++i)
            if(data[i].key > data[i+1].key) {
                printf("Sort failed!\n");
                return -1;
            }
        #ifdef DEBUG
        set<long long> rets;
        for(i = 0; i < n; ++i)  {
            rets.insert((long long)data[i].key);
            // printf("%ld\n", (long long)data[i].key);
        }

        vector<long long> v;
        set_symmetric_difference(
        ints.begin(), ints.end(),
        rets.begin(), rets.end(),
        std::back_inserter(v));
        if(v.size())    printf("FAILED!!!!!!!!!!!!!!");
        #endif
        printf("Sort succesful.\n");


        NDprintf("Time taken: %.2fs\n", end-start);
    }
    return 0;
}