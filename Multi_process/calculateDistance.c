#include <stdio.h>
#include<stdlib.h>
#include "calculateDistance.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>

float Haversize(float Lat1, float Long1, float Lat2, float Long2, float R){
           return (Lat1 + Long1 - Lat2 - Long2); // change this to haversize function
}
void getDistanceMatrix(int shmID, int numberOfProcess,int size,struct node * bucket){
    signal(SIGCHLD, SIG_IGN); // wait for all child-process to exit 
    float * shm;
    if((shm = (float *)shmat(shmID,NULL,0)) == (float *) -1) {
       printf("%s %d","Init:Failed to attach shared memory",shmID);
    }// attach shared memory to current process
    int i = 0;
    int j = 0;
    int count = 1;
    for(count = 1; count < numberOfProcess; count++) {
          pid_t pid = fork();
          if(pid == 0) {// child process
            for(i = count; i < size; i = i + numberOfProcess) {
              for(j = i; j < size; j++) {
                *(shm + size * i + j) = Haversize((bucket + i)->Longitute,(bucket + i)->Latitude,(bucket + j)->Longitute,(bucket + j)->Latitude,2);//        random assign a value to R
                *(shm + size * j + i) =  *(shm + size * i + j);
                //printf("%f ", *(shm + size * i + j));
              }
              //printf("\n");
            }
            if(shmdt(shm) == -1) {
              printf("%s %d %d\n","error occurs when detach share memory",getpid(),count);
              exit(1);
            }
            else {
              printf("%s %d %d\n","successful detach shared memory",getpid(),count);
              exit(0);
            }
          }
    }
    for(i = 0; i < size; i = i + numberOfProcess) { // if-else parent process
      for(j = i; j < size; j = j + 1) {
        *(shm + size * i + j) = Haversize((bucket + i)->Longitute,(bucket + i)->Latitude,(bucket + j)->Longitute,(bucket + j)->Latitude,2);//        random assign a value to R
        *(shm + size * j + i) =  *(shm + size * i + j);
       // printf("%f ", *(shm + size * i + j));
        }
       // printf("\n");
    }
    if(shmdt(shm) == -1) {
      printf("%s %d \n", "error occur share memory in main process", getpid());
    }
    wait(NULL);
    int ret = shmctl(shmID,IPC_RMID,NULL);
    if(ret < 0) {
      printf("%s\n","error occur when delete share memory in main process");
    }
    else {
      printf("%s\n","successful delete share memory in main process");
    }
}
   
  

