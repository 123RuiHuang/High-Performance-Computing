#ifndef __CALCULATEDISTANCE_H__
#define __CALCULATEDISTANCE_H__
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>
struct node { // each row is a record
 float Longitute;
 float Latitude;
};
void getDistanceMatrix(int shmID, int numberOfProcess,int size,struct node * bucket);
float Haversize(float Lat1, float Long1, float Lat2, float Long2, float R);
#endif