#include <stdio.h>
#include "assignToBucket.h"
#include<stdlib.h>
#include <sys/sem.h>  
#include <sys/types.h>
int getMaxIndex(float *array, int row,int column) { // return argnax of this row
     float max = 0;
     int result = 0;
     int j;
     for(j = 4; j < column; j++) { // start from feature 1, the index is 5
        if(*(array + row * column + j) > max) {
             max = *(array + row * column + j);
             result = j - 4;
        }
     }
     return result;
}
void getArgMaxArray(int row, int column, float *array,int* shmOfArgMaxArray,int numOfProcess) {
    signal(SIGCHLD, SIG_IGN); // wait for all child-process to exit 
    int i = 0;
    int count = 1;
    for(count = 1; count < numOfProcess; count++) {
          pid_t pid = fork();
          if(pid == 0) {// child process
             for(i = count; i < row; i = i + numOfProcess) {
              *(shmOfArgMaxArray + i) = getMaxIndex(array,i,column);
            }
            if(shmdt(shmOfArgMaxArray) == -1) {
              printf("%s %d %d\n","error occurs when detach share memory",getpid(),count);
              exit(1);
            }
            else {
             // printf("%s %d %d\n","successful detach shared memory",getpid(),count);
              exit(0);
            }
          }
    }// end of for
    for(i = 0; i < row; i = i + numOfProcess) { // parent process
         *(shmOfArgMaxArray + i) = getMaxIndex(array,i,column);
    }
    wait(NULL);// need to wait 
}

void assignElementToBuckets(float *array, int numberOfcolumn, int numberOfRow, int* shmOfArgMaxArray,struct node * buckets[],int * shmOfBucketsSize,int numOfProcess,int semIds[]) {// each element in buckets is a pointer, point a struct array
    signal(SIGCHLD, SIG_IGN); 
    int i = 0;
    
   
    int count = 1;
    int numOfElementsinEachBucket[21];
    int retval = 1;
    int numOps = 1;
    // (P-op)
    struct sembuf operations_P[1];
    operations_P[0].sem_num = 0; 	// use the first(only, because of semCount above) semaphore
    operations_P[0].sem_flg = 0;	// allow the calling process to block and wait
    operations_P[0].sem_op = -1; 	
    // (V-op)
    struct sembuf operations_V[1];
    operations_V[0].sem_num = 0; 	// use the first(only, because of semCount above) semaphore
    operations_V[0].sem_flg = 0;	// allow the calling process to block and wait
    operations_V[0].sem_op = 1; 
      
    for(i = 0; i < 21; i++) { // initialize two arrays
       *(shmOfBucketsSize + i) = 0;
       numOfElementsinEachBucket[i] = 0;
    }
    

    for(count = 1; count < numOfProcess; count++) {
        pid_t pid = fork();
        if(pid == 0) {
            
            for(i = count; i < numberOfRow; i = i + numOfProcess){
               retval = semop(semIds[*(shmOfArgMaxArray+i)], operations_P, numOps);
               if(retval == 0) {
               //printf("%s\n","success P-op");
               *(shmOfBucketsSize + *(shmOfArgMaxArray + i)) += 1;
               }    // bucketsSize[argmaxarray[i]]++; 
               else {
                printf("%s\n","failed P-op");
                exit(1);
               }      
               retval = semop(semIds[*(shmOfArgMaxArray+i)], operations_V, numOps); 
               if(retval == 0) {
                // printf("%s\n","success V-op");
               } 
               else {
                 printf("%s\n","failed V-op");
                 exit(0);
               }
            }
            if((shmdt(shmOfArgMaxArray) != -1) && (shmdt(shmOfBucketsSize) != -1)) {
             // printf("%s %d %d\n","successful detach shared memory of shmOfArgMaxArray and shmOfBucketsSize",getpid(),count);
              exit(0);
            }
            else {
              printf("%s %d %d\n","error occurs when detach share memory of shmOfArgMaxArray or shmOfBucketsSize",getpid(),count);
              exit(1);
            }
        }
   }// end for
   for(i = 0; i < numberOfRow; i = i + numOfProcess) {
       retval = semop(semIds[*(shmOfArgMaxArray+i)], operations_P, numOps);
       if(retval == 0) {
      // printf("%d\n",*(shmOfBucketsSize + *(shmOfArgMaxArray + i)));
      *(shmOfBucketsSize + *(shmOfArgMaxArray + i))+=1; // bucketsSize[argmaxarray[i]]++;
      }
      else {
        printf("%s\n","failed P-op");
      }
      retval = semop(semIds[*(shmOfArgMaxArray+i)], operations_V, numOps);
      if(retval == 0) {
      }
      else {
       printf("%s\n","failed V-op");
                 exit(0);
      }
      
   }
   wait(NULL);// need to wait 
  
  /*  for(i = 0; i < 21; i++){
    // buckets[i] = (struct node *)malloc(bucketsSize[i]*sizeof(struct node));
    buckets[i] = (struct node *)malloc(*(shmOfBucketsSize + i)*sizeof(struct node));
    }
  
 
    int IDofBucket = 0;
    for(i = 0; i < numberOfRow; i++) {
       
       struct node element = {*(array + i * numberOfcolumn),*(array + i*numberOfcolumn + 1)};// data[i][0],data[i][i]
      
       //IDofBucket = argmaxarray[i];
        IDofBucket = *(shmOfArgMaxArray + i);
        *(buckets[IDofBucket] + numOfElementsinEachBucket[IDofBucket]) = element;
     
       numOfElementsinEachBucket[IDofBucket]++;
       
    }
    int sum = 0;
    for(i = 0; i < 21; i++) {
       //sum = sum + bucketsSize[i];
       sum = sum + *(shmOfBucketsSize + i);
       struct node *bucket = buckets[i];
       int num = *(shmOfBucketsSize + i);
       printf("nodes in this buckets %d \n",num);
       int y = 0;
       //for(y = 0; y < num; y++) {
         //printf("%f %f\n",(bucket+y)->Longitute,(bucket+y)->Latitude);
       //}
     
    
  
     
    }
    //printf("%d",sum);
    
    */
    
    for(i = 0; i < 21; i++) {
       
       int num = *(shmOfBucketsSize + i);
       printf("nodes in this buckets %d \n",num);
       }

}