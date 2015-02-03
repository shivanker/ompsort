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

#endif // _COMMON_H