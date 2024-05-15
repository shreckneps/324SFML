//data.cpp
//Spencer Butler, 11/2/2023, CS324 3D Display

#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#define PI 3.1415926

#include "matrix.h"
#include "data.h"

//Returns the value of a specific function at (x, y)
double DataFunction(double x, double y) {
    double r, numer, denom;
    //r = sqrt(x * x + y * y);
    r = x * x + y * y;
    numer = sin(r) / r;
    denom = 9 * cos(x / (y + 0.02));
    return numer / denom;
}

//Generates shapes representing the top face of a rubik's cube centered at (0, 0, 0)
//Buffer controls space between adjacent squares
//Stores result in output
void DataRubik(double buffer, vector4 output[9][4]) {
    int i, j, k;
    vector4 point;
    matrix4 translationMatrix;
    matrix4 rotationMatrix;
    DefineElementaryTransform(&rotationMatrix, ROTATE_Z, PI / 2);
    double sideLength = (1.0 / 3.0) * (1 - buffer);

    //set point to be topleft of single square centered at (0, 0, 1)
    point[0] = -1 * (sideLength * 0.5);
    point[1] = -1 * (sideLength * 0.5);
    point[2] = 0.5;
    point[3] = 1.0;

    //initialize translationMatrix for center of the upper-left square
    DefineElementaryTransform(&translationMatrix, TRANSLATE_X, (-1.0 / 2.0) + (1.0 / 6.0));
    BuildElementaryTransform(&translationMatrix, TRANSLATE_Y, (-1.0 / 2.0) + (1.0 / 6.0));

    //grid of 3x3 squares
    for(i = 0; i < 3; i++) {
        for(j = 0; j < 3; j++) {
            //each square made of 4 points
            for(k = 0; k < 4; k++) {
                //translate current corner to the current square, store result in output array
                ApplyTransform(point, translationMatrix, &(output[(3 * i) + j][k]));
                //rotate to next corner
                ApplyTransformInPlace(&point, rotationMatrix);
            }
            //move 1/3 over, to center of next square
            BuildElementaryTransform(&translationMatrix, TRANSLATE_X, (1.0 / 3.0));
        }
        //move 1/3 down and 3/3 left, to center of first square on next row
        BuildElementaryTransform(&translationMatrix, TRANSLATE_Y, (1.0 / 3.0));
        BuildElementaryTransform(&translationMatrix, TRANSLATE_X, -1.0);
    }
}

//Generates shapes representing a Tron recognizer
//Number of shapes is stored in count
//Number of points in each shape is stored in sizes
//Shapes are stored in output
void DataRecognizer(vector4 ***output, int **sizes, int *count) {
    int i, j;
    vector4 point;
    point[0] = -1;
    point[1] = 0;
    point[2] = 0;
    point[3] = 1;

    *count = 8;
    (*sizes) = (int *) malloc(sizeof(int) * (*count));
    (*output) = (vector4 **) malloc(sizeof(vector4 *) * (*count));

    (*sizes)[0] = 5;
    (*sizes)[1] = 4;
    (*sizes)[2] = 10;
    for(i = 0; i < 3; i++) {
        (*sizes)[i + 3] = (*sizes)[i];
    }
    (*sizes)[6] = 4;
    (*sizes)[7] = 4;
    for(i = 0; i < 8; i++) {
        (*output)[i] = (vector4 *) malloc(sizeof(vector4) * ((*sizes)[i]));
    }

    //draw little column with trapezoid base
    {
        CopyVector(point, (*output)[0] + 0);

        point[0] = -0.7;
        CopyVector(point, (*output)[0] + 1);

        point[0] = -0.8;
        point[2] = 0.1;
        CopyVector(point, (*output)[0] + 2);

        point[2] = 0.5;
        CopyVector(point, (*output)[0] + 3);

        point[0] = -1;
        CopyVector(point, (*output)[0] + 4);
    }
    
    //draw square above that
    {
        point[2] = 0.7;
        CopyVector(point, (*output)[1] + 0);
        
        point[0] = -0.8;
        CopyVector(point, (*output)[1] + 1);

        point[2] = 0.9;
        CopyVector(point, (*output)[1] + 2);

        point[0] = -1.0;
        CopyVector(point, (*output)[1] + 3);
    }
     
    //draw forkthing above that
    {
        point[0] = -1.1;
        point[2] = 1.0;
        CopyVector(point, (*output)[2] + 0);

        point[0] = -0.75;
        CopyVector(point, (*output)[2] + 1);

        point[0] = -0.6;
        point[2] = 0.75;
        CopyVector(point, (*output)[2] + 2);

        point[0] = -0.1;
        CopyVector(point, (*output)[2] + 3);
            
        point[2] = 0.8;
        CopyVector(point, (*output)[2] + 4);

        point[0] = -0.53;
        CopyVector(point, (*output)[2] + 5);

        point[0] = -0.65;
        point[2] = 1.0;
        CopyVector(point, (*output)[2] + 6);

        point[0] = -0.1;
        CopyVector(point, (*output)[2] + 7);

        point[2] = 1.05;
        CopyVector(point, (*output)[2] + 8);

        point[0] = -1.1;
        CopyVector(point, (*output)[2] + 9);
    }

    //duplicate 3 previous shapes, rotating by pi around z
    {
        matrix4 rotationMatrix;
        DefineElementaryTransform(&rotationMatrix, ROTATE_Z, PI);
        for(i = 0; i < 3; i++) {
            for(j = 0; j < (*sizes)[i]; j++) {
                ApplyTransform((*output)[i][j], rotationMatrix, ((*output)[i + 3] + j));
            }
        }
    }

    //draw long rectangle across center
    {
        point[0] = -1.0;
        point[2] = 0.55;
        CopyVector(point, (*output)[6] + 0);

        point[0] = 1.0;
        CopyVector(point, (*output)[6] + 1);

        point[2] = 0.65;
        CopyVector(point, (*output)[6] + 2);

        point[0] = -1.0;
        CopyVector(point, (*output)[6] + 3);
    }

    //draw big trapezoid up top
    {
        point[0] = -0.65;
        point[2] = 1.1;
        CopyVector(point, (*output)[7] + 0);

        point[0] = 0.65;
        CopyVector(point, (*output)[7] + 1);

        point[0] = 0.2;
        point[2] = 1.2;
        CopyVector(point, (*output)[7] + 2);

        point[0] = -0.2;
        CopyVector(point, (*output)[7] + 3);
    }
}

//Generates shapes representing the word Spencer
//Returns total of 7 shapes
//Number of points in each shape is stored in sizes
void DataSpencer(vector4 *output[7], int sizes[7]) {
    int i;
    vector4 point;
    matrix4 translateMatrix;

    point[0] = 0;
    point[1] = 0;
    point[2] = 0;
    point[3] = 1;

    sizes[0] = 12;
    sizes[1] = 10;
    sizes[2] = 12;
    sizes[3] = 8;
    sizes[4] = 8;
    sizes[5] = sizes[2];
    sizes[6] = 8;
    for(i = 0; i < 7; i++) {
        output[i] = (vector4 *) malloc(sizeof(vector4) * sizes[i]);
    }

    DefineElementaryTransform(&translateMatrix, IDENTITY, 0);
    //s
    {
        vector4 points[12] = {
            {0, 0},
            {0.8, 0},
            {0.8, 0.55},
            {0.1, 0.55},
            {0.1, 0.9},
            {0.8, 0.9},
            {0.8, 1.0},
            {0, 1.0},
            {0, 0.45},
            {0.7, 0.45},
            {0.7, 0.1},
            {0, 0.1}
        };
        for(i = 0; i < sizes[0]; i++) {
            point[0] = points[i][0];
            point[2] = points[i][1];
            ApplyTransform(point, translateMatrix, output[0] + i);
        }
    }

    //p
    {
        BuildElementaryTransform(&translateMatrix, TRANSLATE_X, 1);
        vector4 points[10] = {
            {0, -1},
            {0.1, -1},
            {0.1, 0.9},
            {0.7, 0.9},
            {0.7, 0.1},
            {0.1, 0.1},
            {0.1, 0},
            {0.8, 0},
            {0.8, 1},
            {0, 1}
        };
        for(i = 0; i < sizes[1]; i++) {
            point[0] = points[i][0];
            point[2] = points[i][1];
            ApplyTransform(point, translateMatrix, output[1] + i);
        }
    }
     
    //e
    {
        BuildElementaryTransform(&translateMatrix, TRANSLATE_X, 1);
        vector4 points[12] = {
            {0.1, 0.55},
            {0.7, 0.55},
            {0.7, 0.9},
            {0.1, 0.9},
            {0.1, 0.1},
            {0.8, 0.1},
            {0.8, 0},
            {0, 0},
            {0, 1.0},
            {0.8, 1.0},
            {0.8, 0.45},
            {0.1, 0.45}
        };
        for(i = 0; i < sizes[2]; i++) {
            point[0] = points[i][0];
            point[2] = points[i][1];
            ApplyTransform(point, translateMatrix, output[2] + i);
        }
    }

    //n
    {
        BuildElementaryTransform(&translateMatrix, TRANSLATE_X, 1);
        vector4 points[8] = {
            {0, 0},
            {0, 1},
            {0.8, 1},
            {0.8, 0},
            {0.7, 0},
            {0.7, 0.9},
            {0.1, 0.9},
            {0.1, 0}
        };
        for(i = 0; i < sizes[3]; i++) {
            point[0] = points[i][0];
            point[2] = points[i][1];
            ApplyTransform(point, translateMatrix, output[3] + i);
        }
    }

    //c
    {
        BuildElementaryTransform(&translateMatrix, TRANSLATE_X, 1);
        vector4 points[8] = {
            {0.8, 0},
            {0, 0},
            {0, 1},
            {0.8, 1},
            {0.8, 0.9},
            {0.1, 0.9},
            {0.1, 0.1},
            {0.8, 0.1}
        };
        for(i = 0; i < sizes[4]; i++) {
            point[0] = points[i][0];
            point[2] = points[i][1];
            ApplyTransform(point, translateMatrix, output[4] + i);
        }
    }

    //copy e from output[2]
    {
        BuildElementaryTransform(&translateMatrix, TRANSLATE_X, 1);
        matrix4 offsetMatrix;
        DefineElementaryTransform(&offsetMatrix, TRANSLATE_X, 3);
        for(i = 0; i < sizes[5]; i++) {
            ApplyTransform(output[2][i], offsetMatrix, output[5] + i);
        }
    }

    //r
    {
        BuildElementaryTransform(&translateMatrix, TRANSLATE_X, 1);
        vector4 points[8] = {
            {0, 0},
            {0, 1},
            {0.8, 1},
            {0.8, 0.7},
            {0.7, 0.7},
            {0.7, 0.9},
            {0.1, 0.9},
            {0.1, 0}
        };
        for(i = 0; i < sizes[6]; i++) {
            point[0] = points[i][0];
            point[2] = points[i][1];
            ApplyTransform(point, translateMatrix, output[6] + i);
        }
    }
}

