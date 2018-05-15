# set this to what I want
OTHER_FLAGS=-std=c++0x

# Standard all target
all: hw1_analyze

# HW 1 Analysis Code
hw1_analyze: hw1_analyze.o ../src/MatrixMultiply.o
	g++ -o hw1_analyze hw1_analyze.o ../src/MatrixMultiply.o

hw1_analyze.o:	hw1_analyze.cpp 
	g++ ${OTHER_FLAGS} -Werror -Wall -c hw1_analyze.cpp

clean:
	rm -f *.o hw1_analyze p1.time p1.out


### Analysis Commands and Dependencies

analysis: data
	@echo "Generating Plots:" ; \
	(gnuplot < plot_hw1.gnu && \
	gnuplot < plot_hw1.logxy.gnu && \
	gnuplot < plot_hw1.logx.gnu && \
	gnuplot < plot_hw1.logy.gnu ) 2> /dev/null ; \
	echo "Program Execution Times" ; \
	cat p1.time

data: p1.out
	@echo "Scrapping Timing Statistics:"; ./analyze_1.sh

test_1: hw1_analyze
	@echo "Running hw1_analyze:"; (time ./hw1_analyze > p1.out) 2> p1.time

# this will always run the test_1 because the p1.out 
# always has a time stamp newer that the dependency of test_1
p1.out: test_1

