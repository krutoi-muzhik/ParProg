.PHONY:
	all

all: compile run

compile: HelloWorld.c

	mpicc HelloWorld.c -o HelloWorld

run:
	mpirun -np 4 ./HelloWorld