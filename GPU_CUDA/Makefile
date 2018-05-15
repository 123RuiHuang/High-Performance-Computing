HW5_CPU HW5_GPU cmp:HW5_CPU.cu HW5_GPU.cu cmp.cu
	nvcc -o HW5_CPU HW5_CPU.cu -I /usr/local/cuda/samples/common/inc/
	nvcc -o HW5_GPU HW5_GPU.cu -I /usr/local/cuda/samples/common/inc/
	nvcc -o cmp cmp.cu -I /usr/local/cuda/samples/common/inc/



 
run :
	./HW5_GPU 3 "lena.pgm" "lena_GPU_3.pgm"
	./HW5_GPU 7 "lena.pgm" "lena_GPU_7.pgm"
	./HW5_GPU 11 "lena.pgm" "lena_GPU_11.pgm"
	./HW5_GPU 15 "lena.pgm" "lena_GPU_15.pgm"
	./HW5_CPU 3 "lena.pgm" "lena_CPU_3.pgm"
	./HW5_CPU 7 "lena.pgm" "lena_CPU_7.pgm"
	./HW5_CPU 11 "lena.pgm" "lena_CPU_11.pgm"
	./HW5_CPU 15 "lena.pgm" "lena_CPU_15.pgm"
	./cmp
                                                       

clean :
	rm HW5_GPU lena_* HW5_CPU cmp
