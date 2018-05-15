#include <stdio.h>
#include "assignToBucket.h"
#include<stdlib.h>
#include<pthread.h>
extern int numOfRow ;
extern int numOfColumn ;
extern int bucketsSize[21];
extern pthread_mutex_t mutexs[21];
extern struct node * buckets[21];
extern int curSizeOfEachBuckets[21];

int getMaxIndex(float *data, int row) { // return argnax of this row
     float max = 0;
     int result = 0;
     int j;
     for(j = 4; j < numOfColumn; j++) { // start from feature 1, the index is 5
        if(*(data + row * numOfColumn + j) > max) {
             max = *(data + row * numOfColumn + j);
             result = j - 4;
        }
     }
     return result;
}

void *start_routine(void *arg) {// this is a thread funtion that help to calculate size of each buckets
   struct para *parameter = arg;
   int threadID = parameter->threadID;
   
   int numOfThread = parameter->numOfThread;
   float * data = parameter->data;
   int * argArray = parameter->argArray;
   
   int i = 0,j=0;
    
   for(i = threadID; i < numOfRow; i = i + numOfThread) {
         int bucketID = getMaxIndex(data,i);
         *(argArray + i) = bucketID;
         int * ele = bucketsSize + bucketID;
         pthread_mutex_lock(&(mutexs[bucketID]));
       
             *(ele) = *(ele)+ 1;            //bucketsSize[bucketID]++;
         pthread_mutex_unlock(&(mutexs[bucketID]));
  
         
   }
   pthread_exit(NULL);
   
   
}

void initPara(struct para *parameter, float *data, int threadID,int numOfThread, int * argArray) { // initialize the para that needed to create thread
        parameter->data = data;
        parameter->threadID = threadID;
        parameter->numOfThread = numOfThread;
        parameter->argArray = argArray;
}
void initBucketsSize(float *data,int numOfThread, int * argArray, struct para * parameter,pthread_t *threads){ // use multi-threads to calculate the size of each buckets. The reason I need to know the size of each buckets is that C don't allow me to define a array of struct without dimension.
     int i = 0,j=0;int ret = -1;
     for(i = 0; i < numOfThread; i++) {
        ret = pthread_create(&threads[i],NULL,(void*)start_routine,&parameter[i]);
        if(ret != 0) {
         printf("failed to create thread");
        }
     }
     for(i = 0; i < numOfThread; i++) {
         pthread_join(threads[i],NULL);
     }
     
}

void *assignElements(void *arg) { // this is a thread function, assign element to buckets.
   struct para *parameter = arg;
   int threadID = parameter->threadID;
   int numOfThread = parameter->numOfThread;
   float * data = parameter->data;
   int * argArray = parameter->argArray;
   
   int i = 0,j=0;
    
   for(i = threadID; i < numOfRow; i = i + numOfThread) {
        int bucketID = argArray[i];
        float lon = *(data + i * numOfColumn + 0);
        float lat = *(data + i * numOfColumn + 1);
        float delta_x = *(data + i * numOfColumn + 2);
        float delta_y = *(data + i * numOfColumn + 3);
        float Lon = (delta_x/221220) * cos(lat) + lon;
        float Lat = (delta_y/221220) + lat;
        pthread_mutex_lock(&(mutexs[bucketID]));
        (buckets[bucketID] + curSizeOfEachBuckets[bucketID])->Lon = Lon;
        (buckets[bucketID] + curSizeOfEachBuckets[bucketID])->Lat = Lat;
        curSizeOfEachBuckets[bucketID]++;
        pthread_mutex_unlock(&(mutexs[bucketID]));
         
   }
   
   
}
void assignToBuckets(float *data,int numOfThread,int * argArray) { // assign elements to buckets
    int i = 0,ret = 0;
    
    struct para parameter[numOfThread];
   pthread_t threads[numOfThread];
   for(i = 0; i < numOfThread; i++) {
     initPara(&parameter[i],data,i,numOfThread,argArray);
   }
   initBucketsSize(data,numOfThread,argArray,parameter,threads);
   for(i = 0; i < 21; i++) {
       buckets[i] = (struct node *)malloc(bucketsSize[i] * sizeof(struct node));
   }
   
   for(i = 0; i < numOfThread; i++) {
        ret = pthread_create(&threads[i],NULL,(void*)assignElements,&parameter[i]);
        if(ret != 0) {
         printf("failed to create thread");
        }
   }
   for(i = 0; i < numOfThread; i++) {
         pthread_join(threads[i],NULL);
     }
     for(i = 0; i < 21; i++) {
         pthread_mutex_destroy(&mutexs[i]);
     }

}

