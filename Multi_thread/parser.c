#include <stdio.h>
#include "parser.h"
#include<stdlib.h>
#include<string.h>
extern int numOfColumn;
extern int numOfRow;
void calculateNumOfROWAndColumn(char filename[]) {
   
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
              numOfColumn++;
              pointer = strtok(NULL,seps);
    }
    while(!feof(fp))
    { fscanf(fp,"%s",line);
        numOfRow++;
        //printf("%f",*ROW);
    }

    fclose(fp);

   numOfRow--;
}


void convertCSVToArray(float * data,char filename[]) {
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
              *(data + row * numOfColumn + column) = atof(pointer);
              //printf("%f ",*(data + row * numOfColumn + column));
             
              column++;
              pointer = strtok(NULL,seps);

			   }

        row ++;
     }

    //printf("\n");
    fclose(fp);
    
}
