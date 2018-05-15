#include <stdio.h>
#include <string.h>
#include </usr/local/cuda/samples/common/inc/exception.h>
#include </usr/local/cuda/samples/common/inc/helper_string.h>
#include </usr/local/cuda/samples/common/inc/helper_functions.h>
#include </usr/local/cuda/samples/common/inc/helper_cuda.h>

unsigned int width,height;
__device__ int partition(unsigned char nums[],int begin, int end) {
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

__device__ void quicksort(unsigned char nums[], int begin, int end) {
	if(begin < end) {
		int q = partition(nums,begin,end);
		quicksort(nums,begin,q-1);
		quicksort(nums,q+1,end);
	}

}


__global__ void conv_dev(unsigned char * dev_src, unsigned char * dev_dst, int numRows, int numCols, int KerSize) {
    int x = threadIdx.x + blockIdx.x * blockDim.x;
    
    int y = threadIdx.y + blockIdx.y * blockDim.y;
    
    int offset = x + y * gridDim.x * blockDim.x;
    int padding = (KerSize - 1)/2;
    unsigned char *kernel= (unsigned char*)malloc(KerSize*KerSize*sizeof(unsigned char));
   
    if((x >= padding) && (x < (numCols - padding)) && (y >= padding)&& (y < (numRows - padding))) {
        for(int i = 0; i < KerSize; i++) {
          for(int j = 0; j < KerSize; j++) {
             int xx = x + j - padding;
             int yy = y + i - padding;
             int index1 = i*KerSize + j;
             int index2 = xx + yy * gridDim.x * blockDim.x;
             kernel[index1] = dev_src[index2];
            }
           }
           
           // sort the elem in window
  		quicksort(kernel,0,(KerSize * KerSize) - 1);
  		dev_dst[offset] = kernel[padding * KerSize + padding];
  }
		
    free(kernel);
}


void conv(unsigned char * src, unsigned char * dst, int numRows, int numCols, int kerSize) {
    int totalPixels = numRows * numCols;
    unsigned char *dev_src,*dev_dst;
    checkCudaErrors(cudaMalloc(&dev_src,sizeof(unsigned char)*totalPixels));
    checkCudaErrors(cudaMalloc(&dev_dst,sizeof(unsigned char)*totalPixels));
    dim3 blocks(64,64);
    dim3 threads(8,8);
    cudaEvent_t start, stop;
    cudaEventCreate(&start);
    cudaEventCreate(&stop);
    cudaEventRecord(start, 0);
    
    checkCudaErrors(cudaMemcpy(dev_src,src,sizeof(unsigned char)*totalPixels,cudaMemcpyHostToDevice));
    checkCudaErrors(cudaMemcpy(dev_dst,src,sizeof(unsigned char)*totalPixels,cudaMemcpyHostToDevice));
    
    
    conv_dev<<<blocks,threads>>>(dev_src,dev_dst,numRows,numCols,kerSize);
    checkCudaErrors(cudaMemcpy(dst,dev_dst,sizeof(unsigned char)*totalPixels,cudaMemcpyDeviceToHost));
    
    cudaEventRecord(stop, 0);
    cudaEventSynchronize(stop);
    float elapsedTime;
    cudaEventElapsedTime(&elapsedTime, start, stop);
    printf("filter size: %d, time to copy-compute-copy : %f second\n", kerSize,elapsedTime / 1000.0);
    printf("\n");
    checkCudaErrors(cudaFree(dev_src));
    checkCudaErrors(cudaFree(dev_dst));
    
}







int main(int argc,char* argv[]) {
 int winSize = atoi(argv[1]);
 const char *source_image_Path = argv[2];
 const char *dst_image_Path =  argv[3];
 unsigned char* image_source = NULL;
 unsigned char* image_dst = NULL;  // store the result calculated at GPU
 sdkLoadPGM(source_image_Path,&image_source,&width,&height);
 image_dst = (unsigned char*)malloc(width*height*sizeof(unsigned char));
 conv(image_source,image_dst,width,height,winSize);
 sdkSavePGM(dst_image_Path,image_dst,width,height);
 free(image_dst);
 printf("\n");
}

