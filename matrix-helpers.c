#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

// Helper functions for matrix computation. Works with the matrix struct. To paralellise

typedef struct matrix {
    float **mat;
    int dims[2];
} matrix;


void printm(matrix *m){
    // display a matrix
    for (int i=0; i<m->dims[0]; i++){
        for (int j=0; j<m->dims[1]; j++){
            printf("%f ", m->mat[i][j]);
        }
        printf("\n");
    }

    printf("Dims: %d %d\n\n", m->dims[0], m->dims[1]);
}

matrix *random2(int dims[2]){
    // create a 2d matrix on the heap initialised with random values b/w 0 and 1

    float **ret = (float **)malloc(dims[0]*sizeof(float **));
    for (int i=0; i<dims[0]; i++){
        float *row = (float *)malloc(dims[1]*sizeof(float));
        for (int j=0; j<dims[1]; j++){
            row[j] = (float)rand()/(float)RAND_MAX;
        }

        ret[i] = row;
    }

    matrix *ret1 = (matrix *)malloc(sizeof(matrix));

    ret1->mat = ret;
    ret1->dims[0] = dims[0];
    ret1->dims[1] = dims[1];

    return ret1;
}

matrix *randrange2(int dims[2], int range[2]){
    // create a 2d matrix on the heap initialised with random values b/w a specified range

    if (range[1] < range[0]){
        printf("Error: invalid range\n\n");
        exit(1);
    }

    float **ret = (float **)malloc(dims[0]*sizeof(float **));
    for (int i=0; i<dims[0]; i++){
        float *row = (float *)malloc(dims[1]*sizeof(float));
        for (int j=0; j<dims[1]; j++){
            row[j] = (float)rand()/(float)RAND_MAX * (range[1] - range[0]) + range[0];
        }

        ret[i] = row;
    }

    matrix *ret1 = (matrix *)malloc(sizeof(matrix));

    ret1->mat = ret;
    ret1->dims[0] = dims[0];
    ret1->dims[1] = dims[1];

    return ret1;
}

matrix *ones2(int dims[2]){
    // create a 2d matrix on the heap initialised with random values

    float **ret = (float **)malloc(dims[0]*sizeof(float **));
    for (int i=0; i<dims[0]; i++){
        float *row = (float *)malloc(dims[1]*sizeof(float));
        for (int j=0; j<dims[1]; j++){
            row[j] = 1;
        }

        ret[i] = row;
    }

    matrix *ret1 = (matrix *)malloc(sizeof(matrix));

    ret1->mat = ret;
    ret1->dims[0] = dims[0];
    ret1->dims[1] = dims[1];

    return ret1;
}

matrix *fill2(int dims[2], int val){
    // create a 2d matrix on the heap initialised with custom

    float **ret = (float **)malloc(dims[0]*sizeof(float **));
    for (int i=0; i<dims[0]; i++){
        float *row = (float *)malloc(dims[1]*sizeof(float));
        for (int j=0; j<dims[1]; j++){
            row[j] = val;
        }

        ret[i] = row;
    }

    matrix *ret1 = (matrix *)malloc(sizeof(matrix));

    ret1->mat = ret;
    ret1->dims[0] = dims[0];
    ret1->dims[1] = dims[1];

    return ret1;
}


matrix *zeros2(int dims[2]){
    // create a 2d matrix on the heap initialised with zeros

    float **ret = (float **)malloc(dims[0]*sizeof(float **));
    for (int i=0; i<dims[0]; i++){
        float *row = (float *)calloc(dims[1], sizeof(float));

        ret[i] = row;
    }

    matrix *ret1 = (matrix *)malloc(sizeof(matrix));

    ret1->mat = ret;
    ret1->dims[0] = dims[0];
    ret1->dims[1] = dims[1];

    return ret1;
}

matrix *alloc2(int dims[2]){
    // create an uninitialised 2d matrix on the heap

    float **ret = (float **)malloc(dims[0]*sizeof(float **));
    for (int i=0; i<dims[0]; i++){
        float *row = (float *)malloc(dims[1]*sizeof(float));

        ret[i] = row;
    }

    matrix *ret1 = (matrix *)malloc(sizeof(matrix));

    ret1->mat = ret;
    ret1->dims[0] = dims[0];
    ret1->dims[1] = dims[1];

    return ret1;
}

matrix *matmul(matrix *mat1, matrix *mat2){
    int R1 = mat1->dims[0], C1 = mat1->dims[1];
    int R2 = mat2->dims[0], C2 = mat2->dims[1];

    if (C1 != R2){
        printf("Error: incompatible dimensions for matmul ({%d %d} and {%d %d})\n\n", R1, C1, R2, C2);
        exit(1);
    }

    int rdims[2] = {R1, C2};
    matrix *rslt = alloc2(rdims);

    for (int i = 0; i < R1; i++) {
        for (int j = 0; j < C2; j++) {
            rslt->mat[i][j] = 0;

            for (int k = 0; k < R2; k++) {
                rslt->mat[i][j] += mat1->mat[i][k] * mat2->mat[k][j];
            }
        }
    }

    return rslt;
}

matrix *mul(int n, matrix *mat1){
    float **mat = mat1->mat;
    int *dims = mat1->dims;

    matrix *rslt = alloc2(dims);
    for (int i=0; i<dims[0]; i++){
        for (int j=0; j<dims[1]; j++){
            rslt->mat[i][j] = n*mat[i][j];
        }
    }

    return rslt;
}

matrix *add(matrix *mat1, matrix *mat2){
    int R1 = mat1->dims[0], C1 = mat1->dims[1];
    int R2 = mat2->dims[0], C2 = mat2->dims[1];


    if(R1 != R2 || C1 != C2){
        printf("Error: incompatible dimensions for add ({%d %d} and {%d %d})\n\n", R1, C1, R2, C2);
        exit(1);
    }

    int *dims = mat1->dims;

    matrix *rslt = alloc2(dims);
    for (int i=0; i<dims[0]; i++){
        for (int j=0; j<dims[1]; j++){
            rslt->mat[i][j] = mat1->mat[i][j] + mat2->mat[i][j];
        }
    }

    return rslt;
}