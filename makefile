
output: projectOS_temp.o
	gcc projectOS_temp.c -o output -lncurses

projectOS_temp.o: projectOS_temp.c
	gcc -c projectOS_temp.c 

run:
	./output

clean:
	rm *.o output