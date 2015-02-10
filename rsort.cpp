#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <omp.h>

#include "sort.h"

#define taskfor(j, n, ...) for(int _i = 0; _i < n; ) \
{ \
  int lim = _i+(n)/num_procs; \
  if(lim < _i+1) \
    lim = _i+1; \
  if(lim > (n)) \
    lim = (n); \
  j = _i; \
  PRAGMA(omp task if((n) > for_ser_n) firstprivate(j, lim, ##__VA_ARGS__)) \
  { \
    for(; j < lim; ++j)

#define endtaskfor } _i=lim;} \
  PRAGMA(omp taskwait)

#define PRAGMA(x) _Pragma(#x)

const int ser_n = 1<<12;
int num_procs, for_ser_n;

void psum(int *data, int n) {
  if(n < 2)
    return;

  int *data2 = new int[n>>1];

  int i;
  taskfor(i, n/2, data2, data)
  {
    data2[i] = data[i<<1] + data[(i<<1) + 1];
  } endtaskfor;

  psum(data2, n>>1);

  taskfor(i, n/2, data, data2)
  {
    data[i<<1] = data2[i]-data[(i<<1)+1];
    data[(i<<1)+1] = data2[i];
  } endtaskfor;

  if(n&1)
    data[n-1] += data2[(n>>1)-1];

  delete [] data2;
}

void rSort_helper(dataType *data, int n, int shift, dataType *buffer)    {
    // printf ("%d\n", shift);
    if(n <= 1 || shift < 0) {
      return;
    }

    int i, invert = (shift == 63) ? 1 : 0;
    int *bucket = new int[n];

    taskfor(i, n, bucket, data, shift, invert)  {
      bucket[i] = (((long long)data[i].key >> shift) &1) ^ invert;
    } endtaskfor;

    psum(bucket, n);
    int offset = n-bucket[n-1]-1; // no of zeros - 1
    taskfor(i, n, shift, invert, data, buffer, offset, bucket)   {
      if((((long long)data[i].key >> shift) &1) ^ invert)
        buffer[offset + bucket[i]] = data[i];
      else
        buffer[i-bucket[i]] = data[i];
    } endtaskfor;

    delete [] bucket;

    taskfor(i, n, data, buffer)
    {
      data[i] = buffer[i];
    } endtaskfor;

    i = ++offset; // i = no of zeros
    #pragma omp task if(i > ser_n) firstprivate(data)
    rSort_helper(data, i, shift-1, buffer);

    #pragma omp task if(n-i > ser_n) firstprivate(data, i, n)
    rSort_helper(data+i, n-i, shift-1, buffer+i);
}

void rSort(dataType *data, int n)   {
    omp_set_nested(1);  // for psum
    num_procs = omp_get_num_procs();
    for_ser_n = ((100)*num_procs);
    dataType *newdata = new dataType[n];
    #pragma omp parallel firstprivate(data, n)
    {
        #pragma omp master
        {
            // printf("%d threads\n", omp_get_num_threads());
            rSort_helper(data, n, 63, newdata);
        }
    }

    delete [] newdata;
}
