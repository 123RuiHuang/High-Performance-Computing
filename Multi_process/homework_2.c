#include<stdio.h>
#include<stddef.h>
#include<stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>
#include <time.h>
#include<string.h>
#include <sys/sem.h>

int main(int argc, char * argv[])
{    
     int i = 0;
     int j = 0;
     int numOfProcess = atoi(argv[2]); 
     int numberOfRow = 0;
     int numberOfColumn = 0;
     int bucketsSize[21]; 
     clock_t start1, end1;
     start1 = clock();
     struct node * buckets[21];// each element of buckets is a pointer
     calculateNumberOfROWAndColumn(&numberOfRow,&numberOfColumn,argv[1]);
     float data[numberOfRow][numberOfColumn];
     convertCSVToArray(*data,numberOfColumn,argv[1]);
     end1 = clock();
     double timeofloadingdata = (double)((end1-start1))/CLOCKS_PER_SEC;
     printf("%s %s %f%s\n", "time to load data",argv[1], timeofloadingdata, "s");
      // create a shared memory for argmaxarray and attach it to main process
     int shmIDOfArgMaxArray;
     key_t shmKeyOfArgMaxArray = 123000;// random asign a number
     int shmFlagOfArgMaxArray = IPC_CREAT | 0666;
     if((shmIDOfArgMaxArray = shmget(shmKeyOfArgMaxArray,(sizeof(int) * numberOfRow),shmFlagOfArgMaxArray)) < 0) {
       printf("%s %d","Init:Failed to initialize shared memory of ArgMaxArray",errno);
       exit(1);
     }
     int * shmOfArgMaxArray;
     if((shmOfArgMaxArray = (int *)shmat(shmIDOfArgMaxArray,NULL,0)) == (int *) -1) {
       printf("%s %d","Init:Failed to attach shared memory of ArgMaxArray",shmIDOfArgMaxArray);
     }// end
     
     // create a shared memory for bucketsSize(a array that record the size of each bucket) and attach it to main process
     int shmIDofBucketsSize;
     key_t shmKeyOfBuckets = 123001;
     int shmFlagOfBucketsSize = IPC_CREAT | 0666;
     if((shmIDofBucketsSize = shmget(shmKeyOfBuckets,(sizeof(int) * 21),shmFlagOfBucketsSize)) < 0) {
       printf("%s %d","Init:Failed to initialize shared memory of BucketsSize",errno);
       exit(1);
     }
     int * shmOfBucketsSize;
     if((shmOfBucketsSize = (int *)shmat(shmIDofBucketsSize,NULL,0)) == (int *) -1) {
       printf("%s %d","Init:Failed to attach shared memory of BucketsSize",shmIDOfArgMaxArray);
     }// end 
     
     // create a array of semaphore
     int semIds[21];
     key_t semKeys[21];
     for(i = 0; i < 21; i++) {
       semKeys[i] = 123002 + i;
     }
    
     int semFlag = IPC_CREAT | 0666;
     int semCount = 1;
     int numOps = 1;
     union semun {
          int val;
          struct semid_ds * buf;
          ushort * array;
        }argument;
     argument.val = 1;
     for(i = 0; i < 21; i++) { // create and initialize 21 semaphores
       if ((semIds[i] = semget(semKeys[i], semCount, semFlag)) == -1){
  		    printf("%s %d","Failed to semget",semKeys[i]);
		      exit(1);
	      }
        else{
   		  //  printf("%d\n",semIds[i]);
         // printf("%s %d","Success to semget",semKeys[i]);
	      }// end of create
        
        if( semctl(semIds[i], 0, SETVAL, argument) < 0)
   	    {
	        printf("%s %d","Failed to initialize",semKeys[i]); 
		      exit(1);
	      }
	      else
	      {
		     // printf("%s %d","success initialize",semKeys[i]);
	      }
    }
     
     // end
     clock_t start2, end2;
     printf("%s %d\n","num of process",numOfProcess);
     start2 = clock();
     getArgMaxArray(numberOfRow,numberOfColumn,*data, shmOfArgMaxArray,numOfProcess);
     assignElementToBuckets(*data,numberOfColumn,numberOfRow,shmOfArgMaxArray,buckets,shmOfBucketsSize,numOfProcess,semIds);
     end2 = clock();
     double timeofassign=(double)((end2-start2))/CLOCKS_PER_SEC;
     printf("%s  %f%s\n", "time to assign elements to buckets", timeofassign,"s");
     
    
    
     // detach shmOfArgMaxArray and delete it in main process
     if(shmdt(shmOfArgMaxArray) == -1) {
      printf("%s %d \n", "error occur detach share memory of ArgMaxArray in main process", getpid());
    }
    int ret = shmctl(shmIDOfArgMaxArray,IPC_RMID,NULL);
    if(ret < 0) {
      printf("%s\n","error occur when delete share memory of ArgMaxArray in main process");
    }
    else {
     // printf("%s\n","successful delete share memory of ArgMaxArray in main process");
    } //end
    
    // detach shmOfBucketsSize and delete it in main process
    if(shmdt(shmOfBucketsSize) == -1) {
      printf("%s %d \n", "error occur detach share memory of BucketsSize in main process", getpid());
    }
    ret = shmctl(shmIDofBucketsSize,IPC_RMID,NULL);
    if(ret < 0) {
      printf("%s\n","error occur when delete share memory of BucketsSize in main process");
    }
    else {
      //printf("%s\n","successful delete share memory of BucketsSize in main process");
    } //end
     // release and delete semaphores
     struct sembuf sbuf;
     sbuf.sem_num = 0;
     sbuf.sem_op = 1;
     sbuf.sem_flg = IPC_NOWAIT;
     for(i = 0; i < 21; i++) {
     if(semop(semIds[i],&sbuf,1) == -1) {
         printf("%s\n","semop release error");
      }
      if(semctl(semIds[i],0,IPC_RMID,0) == -1) {
        printf("%s\n","semop delete error");
      }
     }
     /*printf("%s\n","assign over");
     printf("%s\n","begin calculate distance");
     clock_t start,finish;  
     start = clock();
     for(i = 0; i < 21; i++) {
       printf("%s %d\n","bucket",i);
       // create the shared memory for matrix I
          int shmID;
          key_t shmKey = 123459;
          int shmFlag = IPC_CREAT | 0666;// Flag to create with rw permissions
          int size = 0;
          if(bucketsSize[i] > 10000) {
            size = 10000;
          }else {
           size = bucketsSize[i];
          }
          if(size > 0) {
          if((shmID = shmget(shmKey,(size * size * sizeof(float)),shmFlag)) < 0) {
                printf("%s %d","Init:Failed to initialize shared memory",errno);
                exit(1);
          }
          getDistanceMatrix(shmID,numOfProcess,size,buckets[i]);// shared memory will be deleted in this function
          }
          free(buckets[i]);// in function assignElementToBuckets, we malloc space for each buckets.
       }
       finish = clock();
       double TheTimes=(double)((finish-start)/CLOCKS_PER_SEC);
       printf("%s  %f\n", "time period", TheTimes);
       printf("%s","over");
       */
  return 0;
}




