//matrix.h
//Spencer Butler, 11/2/2023, CS324 3D Display

#ifndef matrix_h
#define matrix_h
typedef double vector4[4];
typedef double matrix4[4][4];

#define IDENTITY 0

#define TRANSLATE 1
#define TRANSLATE_X 1
#define TRANSLATE_Y 2
#define TRANSLATE_Z 3

#define ROTATE 4
#define ROTATE_X 4
#define ROTATE_Y 5
#define ROTATE_Z 6

#define PERSPECTIVE 7

//Multiplies point by transform, stores result in output
void ApplyTransform(vector4 point, matrix4 transform, vector4 *output);

//Copies the vector at source to destination
void CopyVector(vector4 source, vector4 *destination);

//Multiplies point by transform, stores result in point
void ApplyTransformInPlace(vector4 *point, matrix4 transform);

//Multiplies left by right, stores result in output
void MultiplyTransforms(matrix4 left, matrix4 right, matrix4 *output);

//Copies the matrix at source to destination
void CopyMatrix(matrix4 source, matrix4 *destination);

//Multiplies initial by additional and stores the results in initial
void ComposeTransform(matrix4 *initial, matrix4 additional);

//Stores a new matrix4 representing the specified transform in output
void DefineElementaryTransform(matrix4 *output, int type, double parameter);

//Modifies initial to also apply the specified transform
void BuildElementaryTransform(matrix4 *initial, int type, double parameter);

//Prints the vector
void printVector(vector4 v);

//Prints the matrix
void printMatrix(matrix4 m);

#endif



