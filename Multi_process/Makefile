homework_2 :homework_2.o parser.o assignToBucket.o calculateDistance.o
	gcc -O2  -g -o homework_2  homework_2.o parser.o assignToBucket.o calculateDistance.o -lm
homework_2.o:homework_2.c 
	gcc -O2  -g -c homework_2.c -lm
parser.o:parser.c
	gcc -O2  -g -c parser.c

assignToBucket.o:assignToBucket.c
	gcc -O2  -g -c assignToBucket.c

calculateDistance.o:calculateDistance.c
	gcc -O2  -g -c calculateDistance.c -lm
clean:
	rm -f *.o homework_2 

