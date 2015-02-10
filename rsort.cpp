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

const int ser_n = 1<<9;
int num_procs, for_ser_n;

void psum(int *data, int n, int *data2) {
  if(n < 2)
    return;

  int i;
  taskfor(i, n/2, data2, data)
  {
    data2[i] = data[i<<1] + data[(i<<1) + 1];
  } endtaskfor;

  psum(data2, n>>1, data2+(n>>1));

  taskfor(i, n/2, data, data2)
  {
    data[i<<1] = data2[i]-data[(i<<1)+1];
    data[(i<<1)+1] = data2[i];
  } endtaskfor;

  if(n&1)
    data[n-1] += data2[(n>>1)-1];
}

void rSort_helper(dataType *data, int n, int shift, dataType *buffer, int *bucket, int *buffer2)    {
    // printf ("%d\n", shift);
    if(n <= 1 || shift < 0)
      return;

    int i, invert = (shift == 63) ? 1 : 0;

    taskfor(i, n)  {
      bucket[i] = (((long long)data[i].key >> shift) &1) ^ invert;
    } endtaskfor;

    psum(bucket, n, buffer2);

    int offset = n-bucket[n-1]-1; // no of zeros - 1
    taskfor(i, n, shift, invert, data, buffer, offset, bucket)   {
      if((((long long)data[i].key >> shift) &1) ^ invert)
        buffer[offset + bucket[i]] = data[i];
      else
        buffer[i-bucket[i]] = data[i];
    } endtaskfor;

    taskfor(i, n)
    {
      data[i] = buffer[i];
    } endtaskfor;

    i = ++offset; // i = no of zeros
    #pragma omp task if(i > ser_n)
    rSort_helper(data, i, shift-1, buffer, bucket, buffer2);

    #pragma omp task if(n-i > ser_n)
    rSort_helper(data+i, n-i, shift-1, buffer+i, bucket+i, buffer2+i);
}

void rSort(dataType *data, int n)   {
    num_procs = omp_get_num_procs();
    for_ser_n = ((1<<9)*num_procs);

    // instead of allocating and deallocating buffers all the time, just allocate once and share
    dataType *newdata = new dataType[n];
    int *bucket = new int[n];
    int *buf2 = new int [n];
    #pragma omp parallel firstprivate(data, n)
    {
        #pragma omp master
        {
            // printf("%d threads\n", omp_get_num_threads());
            rSort_helper(data, n, 63, newdata, bucket, buf2);
        }
    }

    delete [] newdata;
    delete [] bucket;
    delete [] buf2;
}
