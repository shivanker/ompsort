#ifndef _COMMON_H
#define _COMMON_H

#include "sort.h"

#ifdef DEBUG
#define _DEBUG_DEBUG 1
#else
#define _DEBUG_DEBUG 0
#endif

#define Dprintf(fmt, ...) \
        do { if (_DEBUG_DEBUG) \
                fprintf(stderr, "%s:%d: %s(): " fmt, \
                    __FILE__, \
                    __LINE__, \
                    __func__, \
                    ##__VA_ARGS__);\
        } while (0)

#define NDprintf(...) \
        do { if (!_DEBUG_DEBUG) \
                fprintf(stderr, ##__VA_ARGS__);\
        } while (0)

long long randull()   {
    return ((long long)rand() << ((sizeof(int) * 8 - 1) * 2)) | 
           ((long long)rand() << ((sizeof(int) * 8 - 1) * 1)) |
           ((long long)rand() << ((sizeof(int) * 8 - 1) * 0));
}

void apply_perm(dataType *data, int *perm, int n) {
    // dataType tmp;
    // int i, p, p2;
    // for(i = 0; i < n; ++i)
    //     if(perm[i] != -1 && perm[i] != i)    {
    //         tmp = data[i];
    //         p = i;
    //         while(perm[p] != i) {
    //             data[p] = data[perm[p]];
    //             p2 = perm[p];
    //             perm[p] = -1;
    //             p = p2;
    //         }
    //         data[p] = tmp;
    //         perm[p] = -1;
    //     }
    int i;
    dataType *d2 = NULL;

    #pragma omp parallel firstprivate(i, n, data)
    {
        #pragma omp single
        {
            d2 = new dataType[n];
        }

        #pragma omp for
        for(i = 0; i < n; ++i)
            d2[i] = data[i];
        #pragma omp for
        for(i = 0; i < n; ++i)
            data[i] = d2[perm[i]];

        #pragma omp single
        {
            delete [] d2;
        }
    }
}

#endif // _COMMON_H
