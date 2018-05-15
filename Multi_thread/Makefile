homework_3 :homework_3.o parser.o assignToBucket.o calculateDistance.o
	gcc -g -pg -o homework_3  homework_3.o parser.o assignToBucket.o calculateDistance.o -lm -lpthread
homework_3.o:homework_3.c 
	gcc -g -pg -c homework_3.c -lm -lpthread
parser.o:parser.c
	gcc -g -pg -c parser.c

assignToBucket.o:assignToBucket.c 
	gcc -g -pg -c assignToBucket.c -lpthread

calculateDistance.o:calculateDistance.c
	gcc -g -pg -c calculateDistance.c -lm -lpthread
clean:
	rm -f *.o homework_3 

