#include <stdio.h>
#include <string.h>
#include </usr/local/cuda/samples/common/inc/exception.h>
#include </usr/local/cuda/samples/common/inc/helper_string.h>
#include </usr/local/cuda/samples/common/inc/helper_functions.h>
#include </usr/local/cuda/samples/common/inc/helper_cuda.h>
unsigned int width,height;
int main() {
  const char * img_GPU_Path = NULL;
  const char * img_CPU_Path = NULL;

 for(int i = 0; i < 4; i++) {
   if(i == 0) {
    img_GPU_Path = "lena_GPU_3.pgm";
    img_CPU_Path = "lena_CPU_3.pgm";
   }
   if(i == 1) {
     img_GPU_Path = "lena_GPU_7.pgm";
     img_CPU_Path = "lena_CPU_7.pgm";
   }
   if(i == 2) {
     img_GPU_Path = "lena_GPU_11.pgm";
     img_CPU_Path = "lena_CPU_11.pgm";
   }
   if(i == 3) {
     img_GPU_Path = "lena_GPU_15.pgm";
     img_CPU_Path = "lena_CPU_15.pgm";
     }
     
   int counter1 = 0;
   int counter2 = 0;
   unsigned char* img_GPU = NULL;
   unsigned char* img_CPU = NULL;  
   sdkLoadPGM(img_GPU_Path,&img_GPU,&width,&height);
   sdkLoadPGM(img_CPU_Path,&img_CPU,&width,&height);
   for(int m = 0; m < height; m++) {
     for(int n = 0; n < width; n++) {
       if(img_GPU[m * width + n] != img_CPU[m * width + n]) counter1++;
       if(img_GPU[m * width + n] == img_CPU[m * width + n]) counter2++;
     }
   }
   printf("filter size : %d ; Accuracy : %f\n",4*(i+1) - 1,(float)counter2 / (counter1+counter2));
 }
 
}