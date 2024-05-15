//main.cpp
//Spencer Butler, 11/2/2023, CS324 3D Display

#include <stdio.h>

#include "matrix.h"
#include "drawing.h"
#include "data.h"

//Draws a cube with the contents of face on each face
//Each face has its own color
void DrawRubik(vector4 **face, int *sizes, int count, vector4 center);

#define PI 3.1415926
#define RAD(X) (X * PI * 1/180)

#define FUNCTION 1
#define CUBE_SOLID 2
#define CUBE_SPACED 3
#define CUBE_GRID 4
#define RECOGNIZER 5
#define NAME 6

static int cubeColors[6][3] = {
    {255, 0, 0},
    {0, 255, 0},
    {0, 0, 255},
    {255, 0, 255},
    {0, 255, 255},
    {255, 100, 0}
};

//draws things
int main() {
    int i, j, k, choice;
    InitGraphics();
    double vp[4] = {-1, 1, -1, 1};
    SetViewport(vp);

    printf("Select:\n");
    printf("\t 0 -- graph\n");
    printf("\t 1 -- rubik's cube solid\n");
    printf("\t 2 -- rubik's cube gaps\n");
    printf("\t 3 -- many rubik's cubes\n");
    printf("\t 4 -- recognizer\n");
    printf("\t 5 -- name in block letters \n");

    if (scanf_s("%d", &choice) != 1) {
        printf("Error reading drawing choice.\n");
        return 1;
    }


    //graph
    if(choice == 0) {
        double w[4] = {-8, 8, -8, 8};
        SetWindow(w);
        vector4 focalPoint;
        focalPoint[0] = 0;
        focalPoint[1] = 0;
        focalPoint[2] = 0.5;
        focalPoint[3] = 1;
        DefineCameraTransform(focalPoint, 25, RAD(25), RAD(-70), RAD(0));
        
        vector4 p;
        p[0] = 0;
        p[1] = 0;
        p[2] = 0;
        p[3] = 1;

        int steps = 50;

        //draw all lines varying y
        for(p[0] = -2 * PI; p[0] <= 2 * PI; p[0] = p[0] + (4 * PI / steps)) {
            p[1] = -2 * PI;
            p[2] = DataFunction(p[0], p[1]);
            Move3D(p);
            for(; p[1] <= 2 * PI; p[1] = p[1] + (4 * PI / steps)) {
                p[2] = DataFunction(p[0], p[1]);
                Draw3D(p);
            }
        }

        //draw all lines varying x
        for(p[1] = -2 * PI; p[1] <= 2 * PI; p[1] = p[1] + (4 * PI / steps)) {
            p[0] = -2 * PI;
            p[2] = DataFunction(p[0], p[1]);
            Move3D(p);
            for(; p[0] <= 2 * PI; p[0] = p[0] + (4 * PI / steps)) {
                p[2] = DataFunction(p[0], p[1]);
                Draw3D(p);
            }
        }
    }

    //single rubik
    if(choice == 1 || choice == 2) {
        double w[4] = {-1.5, 1.5, -1.5, 1.5};
        SetWindow(w);
        vector4 focalPoint;
        focalPoint[0] = 0;
        focalPoint[1] = 0;
        focalPoint[2] = 0.5;
        focalPoint[3] = 1;
        DefineCameraTransform(focalPoint, 25, RAD(20), RAD(-70), RAD(0));

        vector4 origin;
        origin[0] = 0;
        origin[1] = 0;
        origin[2] = 0;
        origin[3] = 1;
        
        vector4 squares[9][4];
        int fours[9];
        for(i = 0; i < 9; i++) { fours[i] = 4; }
        if(choice == 1) {
            DataRubik(0.0, squares);
        } else {
            DataRubik(0.2, squares);
        }

        vector4 *sq[9];
        for(i = 0; i < 9; i++) { 
            sq[i] = squares[i]; 
        }
        DrawRubik(sq, fours, 9, origin);
    }

    //many rubik
    if(choice == 3) {
        double w[4] = {-4.5, 20, -1.5, 20};
        SetWindow(w);
        vector4 focalPoint;
        focalPoint[0] = 0;
        focalPoint[1] = 0;
        focalPoint[2] = 0.5;
        focalPoint[3] = 1;
        DefineCameraTransform(focalPoint, 30, RAD(20), RAD(-70), RAD(0));

        vector4 origin;
        origin[0] = 0;
        origin[1] = 0;
        origin[2] = 0;
        origin[3] = 1;
        
        vector4 squares[9][4];
        int fours[9];
        for(i = 0; i < 9; i++) { fours[i] = 4; }

        vector4 *sq[9];
        for(i = 0; i < 9; i++) { 
            sq[i] = squares[i]; 
        }
        for(i = 0; i < 8; i++) {
            origin[1] = 0;
            for(j = 0; j < 8; j++) {
                origin[2] = 0;
                for(k = 0; k < 8; k++) {
                    DataRubik(0.2, squares);
                    DrawRubik(sq, fours, 9, origin);
                    origin[2] = origin[2] + 1.5;
                }
                origin[1] = origin[1] + 1.5;
            }
            origin[0] = origin[0] + 1.5;
        }

    }
    
    //recognizer
    if(choice == 4) {
        double w[4] = {-1.5, 1.5, -1.5, 1.5};
        SetWindow(w);
        vector4 focalPoint;
        focalPoint[0] = 0;
        focalPoint[1] = 0;
        focalPoint[2] = 0.5;
        focalPoint[3] = 1;

        DefineCameraTransform(focalPoint, 25, RAD(20), RAD(-75), RAD(0));

        vector4 **shapes;
        int *sizes;
        int count;
        SetColor(128, 0, 64);
        DataRecognizer(&shapes, &sizes, &count);
        for(i = 0; i < count; i++) {
            DrawShape(shapes[i], sizes[i], 0.15, 1);
        }
    }
    
    //spencer
    if(choice == 5) {
        double w[4] = {-1.0, 8.0, -1.5, 1.5};
        SetWindow(w);
        vector4 focalPoint;
        focalPoint[0] = 0.5;
        focalPoint[1] = 0;
        focalPoint[2] = 0.5;
        focalPoint[3] = 1;

        DefineCameraTransform(focalPoint, 20, RAD(5), RAD(-70), RAD(0));

        vector4 *shapes[7];
        int sizes[7];
        DataSpencer(shapes, sizes);
        for(i = 0; i < 7; i++) {
            DrawShape(shapes[i], sizes[i], 0.5, 1);
        }
    }

    return EventLoop();
}

//Draws a cube with the contents of face on each face
//Each face has its own color
void DrawRubik(vector4 **face, int *sizes, int count, vector4 center) {
    int i, j, k;
    int colIndex = 0;
    int *color;

    matrix4 rotationMatrix;
    DefineElementaryTransform(&rotationMatrix, ROTATE_X, PI / 2);

    matrix4 translationMatrix;
    DefineElementaryTransform(&translationMatrix, TRANSLATE_X, center[0]);
    BuildElementaryTransform(&translationMatrix, TRANSLATE_Y, center[1]);
    BuildElementaryTransform(&translationMatrix, TRANSLATE_Z, center[2]);

    matrix4 reverseTranslationMatrix;
    DefineElementaryTransform(&reverseTranslationMatrix, TRANSLATE_X, -1 * center[0]);
    BuildElementaryTransform(&reverseTranslationMatrix, TRANSLATE_Y, -1 * center[1]);
    BuildElementaryTransform(&reverseTranslationMatrix, TRANSLATE_Z, -1 * center[2]);
    
    //four easy faces
    for(i = 0; i < 4; i++) {
        //translate each point
        for(j = 0; j < count; j++) {
            for(k = 0; k < sizes[j]; k++) {
                ApplyTransformInPlace(&(face[j][k]), translationMatrix);
            }
        }

        //draw this face
        color = cubeColors[colIndex];
        SetColor(color[0], color[1], color[2]);
        for(j = 0; j < count; j++) {
            DrawShape(face[j], sizes[j], 0, 0);
        }
        
        //rotate each point
        for(j = 0; j < count; j++) {
            for(k = 0; k < sizes[j]; k++) {
                ApplyTransformInPlace(&(face[j][k]), reverseTranslationMatrix);
                ApplyTransformInPlace(&(face[j][k]), rotationMatrix);
            }
        }
        colIndex++;
    }

    //draw remaining side 1
    DefineElementaryTransform(&rotationMatrix, ROTATE_Y, PI / 2);
    //rotate then translate each point
    for(j = 0; j < count; j++) {
        for(k = 0; k < sizes[j]; k++) {
            ApplyTransformInPlace(&(face[j][k]), rotationMatrix);
            ApplyTransformInPlace(&(face[j][k]), translationMatrix);
        }
    }
    color = cubeColors[colIndex];
    SetColor(color[0], color[1], color[2]);
    for(j = 0; j < count; j++) {
        DrawShape(face[j], sizes[j], 0, 0);
    }

    colIndex++;
    //draw remaining side 2
    //rotate then translate each point
    for(j = 0; j < count; j++) {
        for(k = 0; k < sizes[j]; k++) {
            ApplyTransformInPlace(&(face[j][k]), reverseTranslationMatrix);
            ApplyTransformInPlace(&(face[j][k]), rotationMatrix);
            ApplyTransformInPlace(&(face[j][k]), rotationMatrix);
            ApplyTransformInPlace(&(face[j][k]), translationMatrix);
        }
    }
    color = cubeColors[colIndex];
    SetColor(color[0], color[1], color[2]);
    for(j = 0; j < count; j++) {
        DrawShape(face[j], sizes[j], 0, 0);
    }
}


