all:
	gcc -c matrix.c -fopenmp
	gcc -c perceptron.c -fopenmp
	gcc -c mlp.c -fopenmp
	gcc -c funcs.c -lm -fopenmp

matrix:
	gcc -c matrix.c -fopenmp

matrix-test:
	gcc matrix-test.c matrix.c -o matrix-test.out -fopenmp
	./matrix-test.out

mlp:
	gcc -c mlp.c

mlp-test:
	gcc -c mlp-test.c
	gcc mlp-test.o mlp.o matrix.o -o mlp-test.out -fopenmp
	./mlp-test.out

funcs:
	gcc -c funcs.c -lm -fopenmp

funcs-test:
	gcc -c funcs-test.c -fopenmp
	gcc -lm funcs-test.o funcs.o matrix.o -o funcs-test.out -fopenmp
	./funcs-test.out

perceptron:
	gcc -c perceptron.c

perceptron-test:
	gcc perceptron-test.c perceptron.c matrix.c funcs.c -o perceptron-test.out -fopenmp -lm
	./perceptron-test.out 1 0
	./perceptron-test.out 2 0
	./perceptron-test.out 3 0
	./perceptron-test.out 4 0

perceptron-test-out:
	gcc perceptron-test.c perceptron.c matrix.c funcs.c -o perceptron-test.out -fopenmp -lm
	./perceptron-test.out 1 1

csv-test:
	gcc -c csv-test.c
	gcc matrix.o csv-test.o -o csv-test.out -fopenmp
	./csv-test.out