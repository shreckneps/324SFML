//drawing.cpp
//Spencer Butler, 11/2/2023, CS324 3D Display

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <stdio.h>

#include "drawing.h"

static int device[2];
static double window[4];
static double viewport[4];

static double cursor[2];
static sf::RenderWindow *rw;
static matrix4 camera;
static sf::Color penColor(0, 0, 0, 255);


//Initializes the SFML window and coordinate systems for drawing
void InitGraphics() {
    device[0] = 800;
    device[1] = 800;
    
    rw = new sf::RenderWindow(sf::VideoMode(device[0], device[1]), "3D Display", sf::Style::Close);
    rw->clear(sf::Color::White);

    double wtemp[4] = {-5, 5, -5, 5};
    SetWindow(wtemp);

    double vtemp[4] = {-1, 1, -1, 1};
    SetViewport(vtemp);
}

//Converts a point from window to viewport coordinates
void WindowToViewPort(double before[2], double after[2]) {
    after[0] = lerp(window, before[0], viewport);
    after[1] = lerp(window + 2, before[1], viewport + 2);
}

//Converts a point from screenspace coordinates to drawing-window coordinates (pixels)
void ScreenToDevice(double before[2], double after[2]) {
    double screen[4] = {-1, 1, -1, 1};
    double dev[4] = {0, device[0], 0, device[1]};

    after[0] = (int) lerp(screen, before[0], dev);
    //do device[1] - lerp() to convert from 0-bottom to 0-top
    after[1] = (int) device[1] - lerp(screen + 2, before[1], dev + 2);
}

//Moves the drawing cursor to target without drawing anything
void MoveTo2D(double target[2]) {
    cursor[0] = target[0];
    cursor[1] = target[1];
}

//Draws a line to target, moving the cursor
void DrawTo2D(double target[2]) {
    double temp[2];
    double startPixels[2];
    double endPixels[2];

    //check if start+end+midpoint are all outside the window
    //if they all are, this line normally shouldn't be drawn
    //culling these lines prevents bleedthrough along the window's edges
    midpoint(cursor, target, temp);
    if(!(within(window, cursor) || within(window, temp) || within(window, target))) {
        MoveTo2D(target);
        return;
    }
    
    WindowToViewPort(cursor, temp);
    ScreenToDevice(temp, startPixels);

    WindowToViewPort(target, temp);
    ScreenToDevice(temp, endPixels);

    
    sf::Vertex vertices[2] = {
        sf::Vertex(sf::Vector2f(startPixels[0], startPixels[1]), penColor),
        sf::Vertex(sf::Vector2f(endPixels[0], endPixels[1]), penColor)
    };

    rw->draw(vertices, 2, sf::Lines);

    MoveTo2D(target);
}

//Projects target according to the camera transform, MoveTo2D(results)
void Move3D(vector4 target) {
    vector4 after;
    ApplyTransform(target, camera, &after);
    //double point[2] = {after[0] / after[3], after[1] / after[3]};
    MoveTo2D(&(after[0]));
}

//Projects target according to the camera transform, DrawTo2D(results)
void Draw3D(vector4 target) {
    vector4 after;
    ApplyTransform(target, camera, &after);
    DrawTo2D(&(after[0]));
}

//Draws the closed shape specified by points
//If depth is nonzero, extrudes shape along the chosen axis
void DrawShape(vector4 points[], int size, double depth, int axis) {
    int i;
    vector4 deepPoint;
    matrix4 depthTransform; 
    Move3D(points[0]);
    for(i = 0; i < size; i++) {
        Draw3D(points[i]);
    }
    Draw3D(points[0]);

    if(depth > 0.01 || depth < 0.01) {
        DefineElementaryTransform(&depthTransform, TRANSLATE + axis, depth);

        ApplyTransform(points[0], depthTransform, &deepPoint);
        Move3D(deepPoint);
        for(i = 0; i < size; i++) {
            //draw to next point on deep face
            ApplyTransform(points[i], depthTransform, &deepPoint);
            Draw3D(deepPoint);

            //draw connection to non-deep point
            Move3D(points[i]);
            Draw3D(deepPoint);
        }
        ApplyTransform(points[0], depthTransform, &deepPoint);
        Draw3D(deepPoint);
    }

}

//Sets the pen color
void SetColor(int r, int g, int b) {
    penColor.r = constrain(0, r, 255);
    penColor.g = constrain(0, g, 255);
    penColor.b = constrain(0, b, 255);
}

//Sets viewport coordinates to given bounds (x, x, y, y)
void SetViewport(double bounds[4]) {
    int i;
    order(bounds, bounds + 1);
    order(bounds + 2, bounds + 3);
    for(i = 0; i < 4; i++) {
        if(bounds[i] > 1) {
            viewport[i] = 1;
        } else if (bounds[i] < -1) {
            viewport[i] = -1;
        } else {
            viewport[i] = bounds[i];
        }
    }
}

//Sets window coordinates to given bounds (x, x, y, y)
void SetWindow(double bounds[4]) {
    int i;
    order(bounds, bounds + 1);
    order(bounds + 2, bounds + 3);
    for(i = 0; i < 4; i++) {
        window[i] = bounds[i];
    }
}

//Defines the camera as pointing at focus 
//Camera location defined via spherical coords (r, theta, phi) and orientation alpha
void DefineCameraTransform(vector4 focus, double r, double theta, double phi, double alpha) {
    DefineElementaryTransform(&camera, TRANSLATE_X, -1 * focus[0]);
    BuildElementaryTransform(&camera, TRANSLATE_Y, -1 * focus[1]);
    BuildElementaryTransform(&camera, TRANSLATE_Z, -1 * focus[2]);

    BuildElementaryTransform(&camera, ROTATE_Y, -1 * theta);
    BuildElementaryTransform(&camera, ROTATE_X, phi);
    BuildElementaryTransform(&camera, ROTATE_Z, -1 * alpha);
    
    BuildElementaryTransform(&camera, PERSPECTIVE, r);
}

//Returns 1 if point p is within region r
int within(double r[4], double p[2]) {
    //x is out of bounds
    if(p[0] < fmin(r[0], r[1]) || p[0] > fmax(r[0], r[1])) {
        return 0;
    }

    //y is out of bounds
    if(p[1] < fmin(r[2], r[3]) || p[1] > fmax(r[2], r[3])) {
        return 0;
    }

    return 1;
}

//Calculates midpoint of points p and q, stores in r
void midpoint(double p[2], double q[2], double r[2]) {
    r[0] = (p[0] + q[0]) * 0.5;
    r[1] = (p[1] + q[1]) * 0.5;
}

//Linearly interpolates val, starting within startRange, and ending within endRange
double lerp(double startRange[2], double val, double endRange[2]) {
    order(startRange, startRange + 1);
    order(endRange, endRange + 1);

    if(val < startRange[0]) {
        val = startRange[0];
    } else if(val > startRange[1]) {
        val = startRange[1];
    }

    //map to [0, 1]
    val = (val - startRange[0]) / (startRange[1] - startRange[0]);

    //map to endRange
    val = (val * (endRange[1] - endRange[0])) + endRange[0];
    return val;
}

//Swaps *low and *high if they're out of order
void order(double *low, double *high) {
    double i;
    if(*low > *high) {
        i = *low;
        *low = *high;
        *high = i;
    }
}

//Returns val constrained to within [floor, ceil]
int constrain(int floor, int val, int ceil) {
    if(val < floor)
        return floor;
    if(val > ceil)
        return ceil;
    return val;
}

//Writes name and class information on output
void writeName() {
    sf::Font font;
    if (!font.loadFromFile("C:\\WINDOWS\\FONTS\\TIMES.TTF")) {
        printf("Error loading font.\n");
        return;
    }
    sf::Text text("Spencer Butler, CS324 Fall 2023", font, 20);
    text.setFillColor(sf::Color::Black);
    text.setPosition(25, 35);
    rw->draw(text);
}

//Loops for the lifetime of an SFML render window
int EventLoop() {
    writeName();
    sf::Event event;
    rw->display();
    while (rw->isOpen()) {
        while (rw->pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                rw->close();
            }
        }
    }
    return 0;
}
