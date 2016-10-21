#ifndef CSCIx229
#define CSCIx229

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>

#ifdef USEGLEW
#include <GL/glew.h>
#endif
#define GL_GLEXT_PROTOTYPES
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define Cos(th) cos(3.1415926/180*(th))
#define Sin(th) sin(3.1415926/180*(th))

#ifdef __cplusplus
extern "C" {
#endif

void Print(const char* format , ...);
void Fatal(const char* format , ...);
unsigned int LoadTexBMP(const char* file);
void Project(double fov,double asp,double dim);
void ErrCheck(const char* where);
int  LoadOBJ(const char* file);
void special(int key,int x,int y);
void key(unsigned char ch,int x,int y);
void idle();
void reshape(int width,int height);

// Global Variables

// Display axes
extern int axes;
// Azimuth of view angle
extern int th;
// Elevation of view angle
extern int ph;
// Color mode
extern int mode;
// Dimension of orthogonal box
extern double dim;
// Length of axes
extern double len;
// Maximum number
extern int n;
// Field of view (for perspective)
extern int fov;
// Aspect ratio
extern double asp;
// View mode for print
extern char* views[];
// X-coordinate of camera position
extern double EX;
// Y-coordinate of camera position
extern double EY;
// Z-coordinate of camera position
extern double EZ;
// X-coordinate of where the camera is looking
extern double AX;
// Y-coordinate of where the camera is looking
extern double AY;
// Z-coordinate of where the camera is looking
extern double AZ;

// Lighting
extern int light;
// Ambient light
extern int ambient;
// Diffuse light
extern int diffuse;
// Emission light
extern int emission;
// Specular light
extern int specular;
// Light distance
extern int light_distance;
// Light angle
extern int light_angle;
// Light height
extern int light_height;
// Light move
extern int light_move;


#ifdef __cplusplus
}
#endif

#endif
