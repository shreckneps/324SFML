//drawing.h
//Spencer Butler, 11/2/2023, CS324 3D Display

#ifndef drawing_h
#define drawing_h

#include "matrix.h"

//Initializes the SFML window and coordinate systems for drawing
void InitGraphics();

//Converts a point from window to viewport coordinates
void WindowToViewPort(double before[2], double after[2]);

//Converts a point from screenspace coordinates to drawing-window coordinates (pixels)
void ScreenToDevice(double before[2], double after[2]);

//Moves the drawing cursor to target without drawing anything
void MoveTo2D(double target[2]);

//Draws a line to target, moving the cursor
void DrawTo2D(double target[2]);

//Projects target according to the camera transform, MoveTo2D(results)
void Move3D(vector4 target);

//Projects target according to the camera transform, DrawTo2D(results)
void Draw3D(vector4 target);

//Draws the closed shape specified by points
//If depth is nonzero, extrudes shape along the chosen axis
void DrawShape(vector4 points[], int size, double depth, int axis);

//Sets the pen color
void SetColor(int r, int g, int b);

//Sets viewport coordinates to given bounds (x, x, y, y)
void SetViewport(double bounds[4]);

//Sets window coordinates to given bounds (x, x, y, y)
void SetWindow(double bounds[4]);

//Defines the camera as pointing at focus 
//Camera location defined via spherical coords (r, theta, phi) and orientation alpha
void DefineCameraTransform(vector4 focus, double r, double theta, double phi, double alpha);

//Returns 1 if point p is within region r
int within(double r[4], double p[2]);

//Calculates midpoint of points p and q, stores in r
void midpoint(double p[2], double q[2], double r[2]);

//Linearly interpolates val, starting within startRange, and ending within endRange
double lerp(double startRange[2], double val, double endRange[2]);

//Swaps *low and *high if they're out of order
void order(double *low, double *high);

//Returns val constrained to within [floor, ceil]
int constrain(int floor, int val, int ceil);

//Writes name and class information on output
void writeName();

//Loops for the lifetime of an SFML render window
int EventLoop();

#endif
