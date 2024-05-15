//data.h
//Spencer Butler, 11/2/2023, CS324 3D Display

#ifndef data_h
#define data_h

#include "matrix.h"

//Returns the value of a specific function at (x, y)
double DataFunction(double x, double y);

//Generates shapes representing the top face of a rubik's cube centered at (0, 0, 0)
//Buffer controls space between adjacent squares
//Stores result in output
void DataRubik(double buffer, vector4 output[9][4]);

//Generates shapes representing a Tron recognizer
//Number of shapes is stored in count
//Number of points in each shape is stored in sizes
//Shapes are stored in output
void DataRecognizer(vector4 ***output, int **sizes, int *count);

//Generates shapes representing the word Spencer
//Returns total of 7 shapes
//Number of points in each shape is stored in sizes
void DataSpencer(vector4 *output[7], int sizes[7]);

#endif

