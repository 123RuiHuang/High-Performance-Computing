#ifndef __ASSIGNTOBUCKET_H__
#define __ASSIGNTOBUCKET_H__
#include<pthread.h>
#include<sched.h>
#include "math.h"
#include "common.h"

struct para {
float * data;// csv file
int threadID;
int numOfThread;
int * argArray;
};
void initBucketsSize(float *data,int numOfThread,int * argArray,struct para * parameter,pthread_t * threads);// data refer to the data from csv file
int getMaxIndex(float *data, int row);
void *start_routine(void *arg);
void initPara(struct para *parameter, float *data, int threadID,int numOfThread,int * argArray);
void assignToBuckets(float *data,int numOfThread, int * argArray);
#endif