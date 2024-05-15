//matrix.cpp
//Spencer Butler, 11/2/2023, CS324 3D Display

#include <math.h>
#include <stdio.h>

#include "matrix.h"

//Multiplies point by transform, stores result in output
void ApplyTransform(vector4 point, matrix4 transform, vector4 *output) {
    int i, j;
    for(i = 0; i < 4; i++) {
        (*output)[i] = 0;
        for(j = 0; j < 4; j++) {
            (*output)[i] = (*output)[i] + (point[j] * transform[j][i]);
        }
    }
    for(i = 0; i < 4; i++) {
        (*output)[i] = (*output)[i] / (*output)[3];
    }
}

//Copies the vector at source to destination
void CopyVector(vector4 source, vector4 *destination) {
    int i;
    for(i = 0; i < 4; i++) {
        (*destination)[i] = source[i];
    }
}

//Multiplies point by transform, stores result in point
void ApplyTransformInPlace(vector4 *point, matrix4 transform) {
    vector4 proxy;
    ApplyTransform(*point, transform, &proxy);
    CopyVector(proxy, point);
}

//Multiplies left by right, stores result in output
void MultiplyTransforms(matrix4 left, matrix4 right, matrix4 *output) {
    int i, j, k;
    for(i = 0; i < 4; i++) {
        for(j = 0; j < 4; j++) {
            (*output)[i][j] = 0;
            for(k = 0; k < 4; k++) {
                (*output)[i][j] = (*output)[i][j] + (left[i][k] * right[k][j]);
            }
        }
    }
}

//Copies the matrix at source to destination
void CopyMatrix(matrix4 source, matrix4 *destination) {
    int i, j;
    for(i = 0; i < 4; i++) {
        for(j = 0; j < 4; j++) {
            (*destination)[i][j] = source[i][j];
        }
    }
}

//Multiplies initial by additional and stores the results in initial
void ComposeTransform(matrix4 *initial, matrix4 additional) {
    matrix4 proxy;
    MultiplyTransforms(*initial, additional, &proxy);
    CopyMatrix(proxy, initial);
}

//Returns a new matrix4 representing the specified transform
void DefineElementaryTransform(matrix4 *output, int type, double parameter) {
    int i, j;
    //initialize with identity matrix
    for(i = 0; i < 4; i++) {
        for(j = 0; j < 4; j++) {
            (*output)[i][j] = (i == j);
        }
    }

    switch(type) {
    case TRANSLATE_X:
        (*output)[3][0] = parameter;
        break;
    case TRANSLATE_Y:
        (*output)[3][1] = parameter;
        break;
    case TRANSLATE_Z:
        (*output)[3][2] = parameter;
        break;

    case ROTATE_X:
        (*output)[1][1] = cos(parameter);
        (*output)[1][2] = sin(parameter);
        (*output)[2][1] = -1 * sin(parameter);
        (*output)[2][2] = cos(parameter);
        break;
    case ROTATE_Y:
        (*output)[0][0] = cos(parameter);
        (*output)[0][2] = -1 * sin(parameter);
        (*output)[2][0] = sin(parameter);
        (*output)[2][2] = cos(parameter);
        break;
    case ROTATE_Z:
        (*output)[0][0] = cos(parameter);
        (*output)[0][1] = sin(parameter);
        (*output)[1][0] = -1 * sin(parameter);
        (*output)[1][1] = cos(parameter);
        break;

    case PERSPECTIVE:
        (*output)[2][3] = -1 / parameter;
        break;

    //catches type == identity, and all unexpected entries
    default:
        break;
    }
}

//Modifies initial to also apply the specified transform
void BuildElementaryTransform(matrix4 *initial, int type, double parameter) {
    matrix4 additional;
    DefineElementaryTransform(&additional, type, parameter);
    ComposeTransform(initial, additional);
}

//Prints the vector
void printVector(vector4 v) {
    printf("(%lf, %lf, %lf, %lf)", v[0], v[1], v[2], v[3]);
}

//Prints the matrix
void printMatrix(matrix4 m) {
    int i, j;
    for(i = 0; i < 4; i++) {
        for(j = 0; j < 4; j++) {
            printf("%lf ", m[i][j]);
        }
        printf("\n");
    }
}


