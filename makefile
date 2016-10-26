project: project.o
	cc project.o -o project -lncurses
project.o: project.c
	cc -c project.c 

