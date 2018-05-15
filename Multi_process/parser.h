#ifndef __HELPER_H__
#define __HELPER_H__
#include<string.h>
void calculateNumberOfRowAndColumn(int *numberOfRow, int *numberOfColumn,char filename[]);
void convertCSVToArray(float *p, int numberOfColumn,char filename[]);
void calculateMaxAndMin(float *data, int numberOfRow, int numberOfColumn);
#endif

