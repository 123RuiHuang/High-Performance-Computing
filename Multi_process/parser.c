#include <stdio.h>
#include "parser.h"
#include<stdlib.h>
#include<string.h>
void calculateNumberOfROWAndColumn(int *numberOfRow, int *numberOfColumn,char filename[]) {
   
    char line[1000];

    char * pointer;
    char seps[] = ":";
    //FILE *fp=fopen("hpc_data_starter.csv","r");
    
    FILE *fp=fopen(filename,"r");
    if(!fp)
    {
        printf("can't open file\n");

    }
    fscanf(fp,"%s",line); // header
    pointer = strtok(line,seps);
    while(pointer != NULL) {
              (*numberOfColumn)++;
              pointer = strtok(NULL,seps);
    }
    while(!feof(fp))
    { fscanf(fp,"%s",line);
        (*numberOfRow)++;
        //printf("%f",*ROW);
    }

    fclose(fp);

   (*numberOfRow)--;
   int output = (*numberOfRow)+1;
   //printf("Number of Lines : %d\n", output);
}


void convertCSVToArray(float * p, int numberOfColumn,char filename[]) {

    char line[1000];
    int row = 0, column = 0,i,j;
    char * pointer;
    char seps[] = ":";
    //FILE *fp=fopen("hpc_data_starter.csv","r");
    FILE *fp=fopen(filename,"r");
    if(!fp)
    {
        printf("can't open file\n");

    }
    fscanf(fp,"%s",line);

    while(!feof(fp)) // this loop 9595090 times
    { fscanf(fp,"%s",line);
  //  printf("%d\n",row);
        column = 0;

        pointer = strtok(line,seps);

        while(pointer != NULL) {

              //data[row][column] = atof(pointer);
              *(p + row * numberOfColumn + column) = atof(pointer);
             
              column++;
              pointer = strtok(NULL,seps);

			   }

        row ++;
     }

    //printf("\n");
    fclose(fp);
    
}

void calculateMaxAndMin(float *data,int numberOfRow,int numberOfColumn){
   float max[numberOfColumn],min[numberOfColumn];
   
   int i = 0, j = 0;
   for(i = 0; i < numberOfColumn; i++) {
    max[i] = -32678.0;
    min[i] = 32678.0;// random assign a negative number
   }
   for(i = 0; i < numberOfRow;i++) {
   for(j = 0; j < numberOfColumn; j++) {
      if(max[j] <*(data + i * numberOfColumn + j)) {
       max[j] = *(data + i * numberOfColumn + j);
      }
      if(min[j] >*(data + i * numberOfColumn + j)) {
       min[j] = *(data + i * numberOfColumn + j);
      }
   }
   }
   printf("Maximum number each column: ");
   for(i = 0; i < numberOfColumn; i++) {
     printf("%f ", max[i]);
   }
   printf("\n");
   printf("Minimum number each column: ");
   for(i = 0; i < numberOfColumn; i++) {
     printf("%f ", min[i]);
   }
    printf("\n");
    
}