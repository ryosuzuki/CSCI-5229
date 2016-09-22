/*
 * HW2: Lorenz Attractor
 *
 * Ryo Suzuki
 * rysu7393
 * ryo.suzuki@colorado.edu
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

/*  Lorenz Parameters  */
double s = 10;
double b = 2.6666;
double r = 28;

// Display axes
int axes = 1;
// Azimuth of view angle
int th = 0;
// Elevation of view angle
int ph = 0;
// Color mode
int mode = 0;
// Dimension of orthogonal box
double dim = 60;
// Length of axes
double len = 30;
// Maximum number
int n = 50000;
// Line trace
int current = 0;
double percent = 0;
// Points array
double points[50000][3];
// Colors array
double colors[50000][3];

/*
 *  Convenience routine to output raster text
 *  Use VARARGS to make this more flexible
 */
#define LEN 8192  // Maximum length of text string
void Print(const char* format , ...)
{
  char    buf[LEN];
  char*   ch=buf;
  va_list args;
  // Turn the parameters into a character string
  va_start(args,format);
  vsnprintf(buf,LEN,format,args);
  va_end(args);
  // Display the characters one at a time at the current raster position
  while (*ch)
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,*ch++);
}

/*
 *  Initialize Lorenz points
 *  Integrate 50,000 steps (50 time units with dt = 0.001)
 *  Explicit Euler integration
 */
void init () {
  int i;
  /*  Coordinates  */
  double x = 1;
  double y = 1;
  double z = 1;
  /*  Time step  */
  double dt = 0.001;
  printf("%5d %8.3f %8.3f %8.3f\n",0, x, y, z);

  for (i=0; i<n; i++) {
    double dx = s*(y-x);
    double dy = x*(r-z)-y;
    double dz = x*y - b*z;
    x += dt*dx;
    y += dt*dy;
    z += dt*dz;

    points[i][0] = x;
    points[i][1] = y;
    points[i][2] = z;

    double r;
    double g;
    double b;
    double threshold = (double) n / 3;
    if (mode % 4 == 0) {
      r = 1;
      g = (double) i / n;
      b = (double) i / n;
    } else if (mode % 4 == 1) {
      r = (double) i / n;
      g = 1;
      b = (double) i / n;
    } else if (mode % 4 == 2) {
      r = (double) i / n;
      g = (double) i / n;
      b = 1;
    } else {
      if (i < threshold) {
        r = (double) (n - i) / n;
        g = 1;
        b = 1;
      } else if (i < 2*threshold) {
        r = 1;
        g = (double) (n - i - threshold) / n;
        b = 1;
      } else {
        r = 1;
        g = 1;
        b = (double) (n - i - 2*threshold) / n;
      }
    }
    colors[i][0] = r;
    colors[i][1] = g;
    colors[i][2] = b;
    printf("%5d %8.3f %8.3f %8.3f\n", i+1, x, y, z);
  }
}

/*
 *  OpenGL (GLUT) calls this routine to display the scene
 */
void display () {
  // Erase the window and the depth buffer
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  // Enable Z-buffering in OpenGL
  glEnable(GL_DEPTH_TEST);

  // Set view angle
  glLoadIdentity();
  glRotatef(ph, 1, 0, 0);
  glRotatef(th, 0, 1, 0);

  // Draw Lorenz points
  glBegin(GL_LINE_STRIP);
  for (int i=0; i<current; i++) {
    glColor3d(colors[i][0], colors[i][1], colors[i][2]);
    glVertex3d(points[i][0], points[i][1], points[i][2]);
  }
  glEnd();

  // Draw axes (white)
  glColor3f(1, 1, 1);
  if (axes) {
    glBegin(GL_LINES);
    glVertex3d(0.0, 0.0, 0.0);
    glVertex3d(len, 0.0, 0.0);
    glVertex3d(0.0, 0.0, 0.0);
    glVertex3d(0.0, len, 0.0);
    glVertex3d(0.0, 0.0, 0.0);
    glVertex3d(0.0, 0.0, len);
    glEnd();
    // Label axes
    glRasterPos3d(len, 0.0, 0.0);
    Print("X");
    glRasterPos3d(0.0, len, 0.0);
    Print("Y");
    glRasterPos3d(0.0, 0.0, len);
    Print("Z");
  }


  // Display parameters
  glWindowPos2i(5,5);
  percent = (double) (current * 100) / n;
  Print("Angle=%d,%d  Axes=%s  s=%.2f  b=%.3f  r=%.2f  current=%d/%d (%.1f %%)", th, ph, axes ? "On" : "Off", s, b, r, current, n, percent);

  // Render the scene and make it visible
  glFlush();
  glutSwapBuffers();
}

/*
 *  GLUT calls this routine when an arrow key is pressed
 */
void special(int key, int x, int y) {
  // Right arrow key - increase angle by 5 degrees
  if (key == GLUT_KEY_RIGHT)
    th += 5;
  // Left arrow key - decrease angle by 5 degrees
  else if (key == GLUT_KEY_LEFT)
    th -= 5;
  // Up arrow key - increase elevation by 5 degrees
  else if (key == GLUT_KEY_UP)
    ph += 5;
  // Down arrow key - decrease elevation by 5 degrees
  else if (key == GLUT_KEY_DOWN)
    ph -= 5;
  // Keep angles to +/-360 degrees
  th %= 360;
  ph %= 360;
  // Tell GLUT it is necessary to redisplay the scene
  glutPostRedisplay();
}

/*
 *  GLUT calls this routine when a key is pressed
 */
void key(unsigned char ch, int x, int y) {
  // Exit on ESC
  if (ch == 27)
    exit(0);
  // Reset view angle
  else if (ch == '0') {
    th = ph = 0;
    s = 10;
    b = 2.6666;
    r = 28;
    current = 0;
    init();
  }
  // Toggle axes
  else if (ch == 'a' || ch == 'A')
    axes = 1-axes;
  // Toggle color mode
  else if (ch == 'm' || ch == 'M') {
    mode += 1;
    init();
  }
  // Increase s
  else if(ch == 'S') {
    s += 1;
    init();
  }
  // Decrease s
  else if(ch == 's') {
    s -= 1;
    init();
  }
  // Increase b
  else if(ch == 'B') {
    b += 1;
    init();
  }
  // Decrease b
  else if(ch == 'b') {
    b -= 1;
    init();
  }  // Increase r
  else if(ch == 'R') {
    r += 1;
    init();
  }
  // Decrease r
  else if(ch == 'r') {
    r -= 1;
    init();
  }
  // Tell GLUT it is necessary to redisplay the scene
  glutPostRedisplay();
}

/*
 *  GLUT calls this routine when the window is resized
 */
void reshape(int width,int height)
{
  //  Ratio of the width to the height of the window
  double w2h = (height>0) ? (double)width/height : 1;
  //  Set the viewport to the entire window
  glViewport(0,0, width,height);
  //  Tell OpenGL we want to manipulate the projection matrix
  glMatrixMode(GL_PROJECTION);
  //  Undo previous transformations
  glLoadIdentity();
  //  Orthogonal projection box adjusted for the
  //  aspect ratio of the window
  glOrtho(-dim*w2h,+dim*w2h, -dim,+dim, -dim,+dim);
  //  Switch to manipulating the model matrix
  glMatrixMode(GL_MODELVIEW);
  //  Undo previous transformations
  glLoadIdentity();
}

/*
 *  Incrment current line trace
 */
void idle () {
  if (current < n)
    current += 10;
    glutPostRedisplay();
}

/*
 *  Start up GLUT and tell it what to do
 */
int main (int argc, char *argv[]) {
  init();

  // Initialize GLUT and process user parameters
  glutInit(&argc,argv);
  // Request double buffered, true color window with Z buffering
  glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
  // Request 800 x 800 pixel window
  glutInitWindowSize(800, 800);
  // Create the window
  glutCreateWindow("Lorenz - Ryo Suzuki");
  // Tell GLUT to call "display" when the scene should be drawn
  glutDisplayFunc(display);
  // Tell GLUT to call "reshape" when the window is resized
  glutReshapeFunc(reshape);
  // Tell GLUT to call "special" when an arrow key is pressed
  glutSpecialFunc(special);
  // Tell GLUT to call "key" when a key is pressed
  glutKeyboardFunc(key);
  // Tell GLUT to call "idle" when nothing else is going on
  glutIdleFunc(idle);
  // Pass control to GLUT so it can interact with the user
  glutMainLoop();
  // Return code

  return 0;
}
