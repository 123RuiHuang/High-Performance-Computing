#include <stdio.h>
#include <string.h>
#include <time.h>
#include </usr/local/cuda/samples/common/inc/helper_string.h>
#include </usr/local/cuda/samples/common/inc/helper_functions.h>
#include </usr/local/cuda/samples/common/inc/helper_cuda.h>


unsigned int width,height;

int partition(unsigned char nums[],int begin, int end) {
	int i = begin - 1;
	unsigned char media = nums[end];
	for(int j = begin; j < end; j++) {
		if(nums[j] < media) {
			i++;
		unsigned	char temp = nums[i];
			nums[i] = nums[j];
			nums[j] = temp;
		}
		
	}
	i++;
	unsigned char temp = nums[i];
	nums[i] = nums[end];
	nums[end] = temp;
  return i;
}


void quicksort(unsigned char nums[], int begin, int end) {
	if(begin < end) {
		int q = partition(nums,begin,end);
		quicksort(nums,begin,q-1);
		quicksort(nums,q+1,end);
	}

}






void conv(unsigned char * src, unsigned char * dst, int numRows, int numCols, int kerSize) {
        
        for(int i = 0; i < numRows;i++) {// copy src to dst
          for(int j = 0; j < numCols; j++) {
              dst[i * numCols + j] = src[i * numCols + j];
          }
        }
        unsigned char *kernel= (unsigned char*)malloc(kerSize*kerSize*sizeof(unsigned char));
        int padding = (kerSize - 1)/2;
        for(int i = 0; i < numRows; i++) { //ith row
          for(int j = 0; j < numCols;j++) { //jth column
            if((i >= padding) && (i < (numRows - padding)) && (j >= padding)&& (j < (numCols - padding))) {
                  for(int ii = 0; ii < 2 * padding + 1; ii++) {
                    for(int jj = 0; jj <  2 * padding + 1; jj++) {
                        kernel[ii * kerSize + jj] = src[(i-padding + ii)*numCols + (j-padding +jj)];
                       // printf("%d \n",ii * kerSize + jj);
                    }
                  }
                  quicksort(kernel,0,(kerSize * kerSize) - 1);
                  dst[i*numCols + j] = kernel[padding * kerSize + padding];
            }
          }
        }
        free(kernel);
}





int main(int argc,char* argv[]) {
 int winSize = atoi(argv[1]);
 const char *source_image_Path = argv[2];
 const char *dst_image_Path =  argv[3];
 unsigned char* image_source = NULL;
 unsigned char* image_dst = NULL;  // store the result calculated at CPU
 sdkLoadPGM(source_image_Path,&image_source,&width,&height);

 image_dst = (unsigned char*)malloc(width*height*sizeof(unsigned char));
 clock_t start = clock();
 conv(image_source,image_dst,width,height,winSize);
 clock_t end = clock();
 printf("filter size : %d, time : %f second\n", winSize, (double)(end-start)/CLOCKS_PER_SEC);
 sdkSavePGM(dst_image_Path,image_dst,width,height);
 free(image_dst);
 printf("\n");

}

