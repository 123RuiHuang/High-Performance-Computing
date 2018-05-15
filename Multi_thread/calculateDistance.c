#include <stdio.h>
#include<stdlib.h>
#include "calculateDistance.h"
#include<time.h>
extern int numOfRow;
extern int numOfColumn;
extern int bucketsSize[21];
extern struct node * buckets[21];
#define PI 3.14
float Haversize(struct node node1, struct node node2){// return distance between two points
           float result = 0;
           int R = 6371000;
           float Lon1 = node1.Lon*PI/180,Lat1 = node1.Lat*PI/180, Lon2 = node2.Lon*PI/180, Lat2 = node2.Lat*PI/180; 
           result = 2 * R *asin(   sqrt(pow(sin((Lat2 - Lat1)/2),2) + cos(Lat1) * cos(Lat2) * pow(sin((Lon2 - Lon1)/2),2) ) );
           return result;
}
void *threadFunction(void *arg) {// thread function that help to calculate distance matrix
   struct para2 *parameter = arg;
   int threadID = parameter->threadID;
   int numOfThread = parameter->numOfThread;
   int bucketID = parameter->bucketID;
   float * distanceMatrix = parameter->distanceMatrix;
   int size = bucketsSize[bucketID];
   int i = 0,j=0;
   float sum = 0; // calculate average distance;
   for(i = threadID; i < size; i = i + numOfThread) {
         for(j = i+1; j < size; j++) { 
            float distance = Haversize(*(buckets[bucketID] + i), *(buckets[bucketID] + j));
            *(distanceMatrix + i * size + j) = distance;
              sum = sum + distance;
         }
         
   }
  
   parameter->sum = sum;
}
void calculateDistanceMatrixForEachBucket(int numOfThread, int bucketID) {// calculate the distance matrix of the bucket whose ID is the parameter bucketID
          struct para2 parameter[numOfThread];
          pthread_t threads[numOfThread];
          int size = bucketsSize[bucketID], i = 0,ret = 0;
          float distance[size][size];
         
          float averageDistance = 0.0;
          for(i = 0; i < numOfThread; i++) {
               parameter[i].numOfThread = numOfThread;
               parameter[i].threadID = i;
               parameter[i].bucketID = bucketID;
               parameter[i].distanceMatrix = *distance;
               parameter[i].sum = 0;
          }
          for(i = 0; i < numOfThread; i++) {
              ret = pthread_create(&threads[i],NULL,(void*)threadFunction,&parameter[i]);
              if(ret != 0) {
                 printf("failed to create thread");
              }
          }
           for(i = 0; i < numOfThread; i++) {
            pthread_join(threads[i],NULL);
         } 
         for(i = 0; i < numOfThread; i++) {
           averageDistance += parameter[i].sum;
         }
       //printf(" %s %f ", "average distance :", (averageDistance*2)/(size * size));
         
         
          

}
void calculateDistanceMatrixForBuckets(int numOfThread){ // go through 21 buckets and calculate the distance of buckets whose size is less than 5001
    int i = 0;
    for(i = 0; i < 21; i++){
      if(bucketsSize[i] <= 5001) {
       calculateDistanceMatrixForEachBucket(numOfThread,i);
     }
    }
}
   
  

