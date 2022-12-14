#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

#include "matrix.h"
#include "funcs.h"
#include "perceptron.h"

perceptron * perceptron_init(
    matrix *(*activation_function)(matrix *),
    matrix *(*derivative_func)(matrix *),
    double learning_rate,
    int input_dims
) {
    perceptron *model = (perceptron *)malloc(sizeof(perceptron));
    model->learning_rate = learning_rate;
    model->activation_function = activation_function;
    model->derivative_func = derivative_func;
    model->weights = randrange2(
        (int[]){1, input_dims},
        (int[]){-10, 10}
    );
    model->bias = rand()/RAND_MAX;
    return model;
}

matrix *_perceptron_evaluate(perceptron *model, matrix *X){
    matrix *X_T = transpose(X);
    matrix *Z_T = matmul(model->weights, X_T);
    matrix *Z_Tb = broadadd(Z_T, model->bias);
    // matrix *Y = model->activation_function(Z_Tb);
    return Z_Tb; 
}

void perceptron_train(perceptron * model, matrix *X[], double Y[], int sample_size, int num_iter, bool show_output) {
    for (int epoch=0; epoch<num_iter; epoch++){
        double mse = 0;
        double m_dloss_db = 0;
        matrix *m_dloss_dW = zeros2(model->weights->dims);

        for (int i=0; i<sample_size; i++){
            double y_pred = perceptron_predict(model, X[i]);
            double loss = pow((Y[i] - y_pred), 2);
            double dloss_dypred = -2*(Y[i] - y_pred);
            double dypred_dZ = model->derivative_func(_perceptron_evaluate(model, X[i]))->mat[0][0];

            matrix *dZ_dW = X[i];
            double dZ_db = 1;
            
            double dloss_dZ = dloss_dypred * dypred_dZ;

            double dloss_db = dloss_dZ * dZ_db;
            matrix *dloss_dW = mul(dloss_dZ, dZ_dW);

            // printf("dloss_dypred: %f    dypred_dZ: %f\n", dloss_dypred, dypred_dZ);
            
            mse += loss;
            m_dloss_db += dloss_db;
            m_dloss_dW = add(m_dloss_dW, dloss_dW);
            
        }
        mse /= sample_size;
        m_dloss_db /= sample_size;
        m_dloss_dW = mul((double)1/sample_size, m_dloss_dW);

        model->bias -= m_dloss_db*model->learning_rate;
        model->weights = subtract(model->weights, mul(model->learning_rate, m_dloss_dW));
        // model->weights = subtract(model->weights, m_dloss_dW);

        // printm(model->weights); 
        // printf("%f", model->bias); printf("\n");

        if (show_output) printf("Epoch: %d  --  Loss: %f\n", epoch, mse);

    }
}

double perceptron_predict(perceptron *model, matrix *X){
    matrix *X_T = transpose(X);
    matrix *Z_T = matmul(model->weights, X_T);
    matrix *Z_Tb = broadadd(Z_T, model->bias);
    matrix *Y = model->activation_function(Z_Tb);
    return Y->mat[0][0]; 
}


double predict_evaluate(perceptron *model, matrix *X[], double Y[], int sample_size){
    double accuracy = 0;
    int pos = 0, predi;
    for (int i=0; i<sample_size; i++){
        double pred = perceptron_predict(model, X[i]);
        double test = pred - Y[i];
        if (test < 0) test = -test;
        if (pred - Y[i] < 0.5){
            pos++;
        }
        
        if(Y[i] == predi) pos++;
    }
    return (double)pos/sample_size;
}