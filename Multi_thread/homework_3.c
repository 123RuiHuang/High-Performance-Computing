#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<pthread.h>
#include<sched.h>
#include "common.h"
int numOfRow = 0;
int numOfColumn = 0;
int bucketsSize[21];
pthread_mutex_t mutexs[21];
struct node * buckets[21];
int curSizeOfEachBuckets[21];
int main(int argc, char * argv[])
{    printf("%s %s    %s %s\n", "file name :",argv[1],"number of threads:",argv[2]);
     struct timespec start1,finish1,start2, finish2; // time 
     int i = 0;
     int j = 0;
     numOfColumn = 0;
     numOfRow = 0;
     for(i = 0; i < 21; i++) {// initialize
       bucketsSize[i] = 0;
       curSizeOfEachBuckets[i] = 0;
       pthread_mutex_init(&mutexs[i],NULL);
     }
     int numOfThread = atoi(argv[2]); 
     calculateNumOfROWAndColumn(argv[1]);
     float data[numOfRow][numOfColumn];
     clock_gettime(CLOCK_MONOTONIC,&start1);
     convertCSVToArray(*data,argv[1]);
     clock_gettime(CLOCK_MONOTONIC,&finish1);
     double time1 = 1000000*(finish1.tv_sec - start1.tv_sec);
     time1 += ((finish1.tv_nsec - start1.tv_nsec)/1000);
     printf("%s %f\n", "time(ms) to load data:", time1/1000);
     int argArray[numOfRow];
     assignToBuckets(*data,1,argArray);// second parameter is the number of thread. I implement a multi-thread method to assign each element to buckets, but according to the homework pseudocode, we should only parallelize the part that calculate distance mastrix, so I set the second parameter to 1
     clock_gettime(CLOCK_MONOTONIC,&start2);
     calculateDistanceMatrixForBuckets(numOfThread);
     clock_gettime(CLOCK_MONOTONIC,&finish2);
     double time2 = 1000000*(finish2.tv_sec - start2.tv_sec);
     time2 += ((finish2.tv_nsec - start2.tv_nsec)/1000);
     printf("%s %f\n", "time(ms) to calculate distance matrix and compute average distance", time2/1000);
     for(i = 0; i < 21; i++) {
       free(buckets[i]);
   }
   printf("\n");
  return 0;
}




