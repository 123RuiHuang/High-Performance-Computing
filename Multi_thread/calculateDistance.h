#ifndef __CALCULATEDISTANCE_H__
#define __CALCULATEDISTANCE_H__
#include "math.h"
#include "common.h"
#include<pthread.h>
extern int numOfRow;
extern int numOfColumn;
extern int bucketsSize[21];
extern pthread_mutex_t mutexs[21];
extern struct node * buckets[21];
extern int curSizeOfEachBuckets[21];
struct para2 {
  int numOfThread;
  int threadID;
  int bucketID;
  float * distanceMatrix;
  float sum;
};
void  calculateDistanceMatrixForBuckets(int numOfThread);
float Haversize(struct node node1, struct node node2);
#endif