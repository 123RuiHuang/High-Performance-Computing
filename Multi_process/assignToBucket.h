#ifndef __ASSIGNTOBUCKET_H__
#define __ASSIGNTOBUCKET_H__
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>
struct node { // each row is a record
 float Longitute;
 float Latitude;
};
void getArgMaxArray(int row,int column,float *array, int* shm,int numOfProcess);
int getMaxIndex(float *array, int row,int column);
void assignElementToBuckets(float *array, int column,int numberOfRow,int* shm,struct node * buckets[],int bucketsSize[],int numOfProcess,int semIds[]);
#endif