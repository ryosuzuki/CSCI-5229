/*
 * HW5: Lighting
 *
 * Ryo Suzuki
 * rysu7393
 * ryo.suzuki@colorado.edu
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif


// Display axes
int axes = 1;
// Azimuth of view angle
int th = 0;
// Elevation of view angle
int ph = 0;
// Color mode
int mode = 0;
// Dimension of orthogonal box
double dim = 4.5;
// Length of axes
double len = 2;
// Maximum number
int n = 50000;
// Field of view (for perspective)
int fov = 80;
// Aspect ratio
double asp=1;
// View mode for print
char* views[] = {"Orthogonal", "Perspective", "First Person"};
// X-coordinate of camera position
double EX = 0;
// Y-coordinate of camera position
double EY = 0;
// Z-coordinate of camera position
double EZ = 10;
// X-coordinate of where the camera is looking
double AX = 0;
// Y-coordinate of where the camera is looking
double AY = 0;
// Z-coordinate of where the camera is looking
double AZ = 0;

// Lighting
int light = 1;
// Ambient light
int ambient = 30;
// Diffuse light
int diffuse = 100;
// Emission light
int emission = 0;
// Specular light
int specular = 0;
// Light distance
int light_distance = 5;
// Light angle
int light_angle = 90;
// Light height
int light_height = 3;
// Light move
int light_move = 1;


//  Cosine and Sine in degrees
#define Cos(x) (cos((x)*3.1415927/180))
#define Sin(x) (sin((x)*3.1415927/180))

/*
 *  Check for OpenGL errors
 */
void ErrCheck (const char* where) {
  int err = glGetError();
  if (err) fprintf(stderr,"ERROR: %s [%s]\n",gluErrorString(err),where);
}

/*
 *  Convenience routine to output raster text
 *  Use VARARGS to make this more flexible
 */
#define LEN 8192  //  Maximum length of text string
void Print (const char* format , ...) {
  char    buf[LEN];
  char*   ch=buf;
  va_list args;
  //  Turn the parameters into a character string
  va_start(args,format);
  vsnprintf(buf,LEN,format,args);
  va_end(args);
  //  Display the characters one at a time at the current raster position
  while (*ch)
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,*ch++);
}


/*
 *  Draw a sphere at (x,y,z) radius (r)
 */
void Sun (double x, double y, double z, double radius) {
  const int d=5;
  //  Save transformation
  glPushMatrix();
  //  Offset and scale
  glTranslatef(x, y, z);
  glScalef(radius, radius, radius);

  glColor3f(1, 1, 1);
  float shininess[] = {0};
  float color[] = {1, 1, 1, 1.0};
  float emit[] = {0.0, 0.0, 0.01*emission, 1.0};
  glMaterialfv(GL_FRONT,GL_SHININESS, shininess);
  glMaterialfv(GL_FRONT,GL_SPECULAR, color);
  glMaterialfv(GL_FRONT,GL_EMISSION, emit);

  for (int ph=-90; ph<90; ph+=d) {
    glBegin(GL_QUAD_STRIP);
    for (int th=0; th<=360; th+=d) {
      double x_1 = Sin(th)*Cos(ph);
      double y_1 = Cos(th)*Cos(ph);
      double z_1 = Sin(ph);
      glNormal3d(x_1, y_1, z_1);
      glVertex3d(x_1, y_1, z_1);

      double x_2 = Sin(th)*Cos(ph+d);
      double y_2 = Cos(th)*Cos(ph+d);
      double z_2 = Sin(ph+d);
      glNormal3d(x_2, y_2, z_2);
      glVertex3d(x_2, y_2, z_2);
    }
    glEnd();
  }

  //  Undo transformations
  glPopMatrix();
}



void Cylinder (double x, double y, double z, double radius, double height) {
  const int d=5;
  //  Save transformation
  glPushMatrix();
  //  Offset and scale
  glTranslatef(x, y, z);
  glScalef(radius, height, radius);

  glBegin(GL_TRIANGLE_FAN);
  glVertex3f(0, 0, 0);
  for (int th=0; th<=360; th+=d) {
    glColor3f(0.4*Cos(th), 0.4 , 0.4*Sin(th));
    glNormal3d(Sin(th), 0, Cos(th));
    glVertex3f(Sin(th), 0, Cos(th));
  }
  glEnd();

  // Latitude bands
  for (int th=0; th<=360; th+=d) {
    glBegin(GL_QUADS);
    glColor3f(0.4*Cos(th), 0.4 , 0.4*Sin(th));

    float shininess[] = {0};
    float color[] = {0.4*Cos(th), 0.4, 0.4*Sin(th), 1.0};
    float emit[]  = {0.0, 0.0, 0.01*emission, 1.0};
    glMaterialfv(GL_FRONT,GL_SHININESS, shininess);
    glMaterialfv(GL_FRONT,GL_SPECULAR, color);
    glMaterialfv(GL_FRONT,GL_EMISSION, emit);

    glVertex3f(Sin(th), 0, Cos(th));
    glVertex3f(Sin(th), 1, Cos(th));
    glVertex3f(Sin(th+d), 1, Cos(th+d));
    glVertex3f(Sin(th+d), 0, Cos(th+d));
    glEnd();
  }

  glBegin(GL_TRIANGLE_FAN);
  glVertex3f(0, 1, 0);
  for (int th=0; th<=360; th+=d) {
    glColor3f(0.4*Cos(th), 0.4 , 0.4*Sin(th));
    glVertex3f(Sin(th), 1, Cos(th));
  }
  glEnd();
  //  Undo transformations
  glPopMatrix();
}


/*
 *  Draw a sphere at (x,y,z) radius (r)
 */
void Cone (double x, double y, double z, double radius, double height) {
  const int d=5;
  //  Save transformation
  glPushMatrix();
  //  Offset and scale
  glTranslatef(x, y, z);
  glScalef(radius, height, radius);

  glBegin(GL_TRIANGLE_FAN);
  glVertex3f(0, 0, 0);
  for (int th=0; th<=360; th+=d) {
    glColor3f(0.5*Cos(th), 1 , 0.5*Sin(th));

    float shininess[] = {0};
    float color[] = {0.5*Cos(th), 1, 0.5*Sin(th), 1.0};
    float emit[]  = {0.0, 0.0, 0.01*emission, 1.0};
    glMaterialfv(GL_FRONT,GL_SHININESS, shininess);
    glMaterialfv(GL_FRONT,GL_SPECULAR, color);
    glMaterialfv(GL_FRONT,GL_EMISSION, emit);

    glNormal3d(Sin(th), 0, Cos(th));
    glVertex3f(Sin(th), 0, Cos(th));
  }
  glEnd();

  glBegin(GL_TRIANGLE_FAN);
  glVertex3f(0, 1, 0);
  for (int th=0; th<=360; th+=d) {
    glColor3f(0.5*Cos(th), 1 , 0.5*Sin(th));

    float shininess[] = {0};
    float color[] = {0.5*Cos(th), 1, 0.5*Sin(th), 1.0};
    float emit[]  = {0.0, 0.0, 0.01*emission, 1.0};
    glMaterialfv(GL_FRONT,GL_SHININESS, shininess);
    glMaterialfv(GL_FRONT,GL_SPECULAR, color);
    glMaterialfv(GL_FRONT,GL_EMISSION, emit);

    glNormal3d(Sin(th), 0, Cos(th));
    glVertex3f(Sin(th), 0, Cos(th));
  }
  glEnd();

  //  Undo transformations
  glPopMatrix();
}

/*
 *  Draw a triangle at (x,y,z)
 *  with height (h), length (l), width (w)
 *  at angle (angle) on x (ax), y (ay), or z (az)
 */
// void Triangle(double x, double y, double z, double width, double height, double depth) {
//   //  Save transformation
//   glPushMatrix();
//   // Offset, scale, and rotate
//   glTranslatef(x, y, z);
//   glScalef(width, height, depth);
//   // glRotatef(angle, ax, ay, az);
//   // Draw a triangle
//   glBegin(GL_TRIANGLES);
//   glVertex3f(1, 0, 0);
//   glVertex3f(0, 1, 0);
//   glVertex3f(0, 0, 0);
//   glEnd();
//   //  Undo transofrmations
//   glPopMatrix();
// }

/*
 *  Draw a cube at (x,y,z)
 *  with height (h), length (l), width (w)
 *  at angle (angle) on x (ax), y (ay), or z (az)
 *
 */
void Cube(double x, double y, double z, double width, double height, double depth, double angle, double ax, double ay, double az) {
  //  Save transformation
  glPushMatrix();
  //  Offset, scale
  glTranslatef(x, y, z);
  glRotatef(angle, ax, ay, az);
  glScalef(width, height, depth);
  glColor3f(0, 0, 1);


  float shininess[] = {0};
  float color[] = {0, 0, 1, 1.0};
  float emit[]  = {0.0, 0.0, 0.01*emission, 1.0};
  glMaterialfv(GL_FRONT,GL_SHININESS, shininess);
  glMaterialfv(GL_FRONT,GL_SPECULAR, color);
  glMaterialfv(GL_FRONT,GL_EMISSION, emit);


  //  Cube
  glBegin(GL_QUADS);
  glNormal3f( 0, 0,+1);
  glVertex3f(-1, 0,+1);
  glVertex3f(+1, 0,+1);
  glVertex3f(+1,+1,+1);
  glVertex3f(-1,+1,+1);
  //  Back
  glNormal3f( 0, 0,-1);
  glVertex3f(+1, 0,-1);
  glVertex3f(-1, 0,-1);
  glVertex3f(-1,+1,-1);
  glVertex3f(+1,+1,-1);
  //  Right
  glNormal3f(+1, 0, 0);
  glVertex3f(+1, 0,+1);
  glVertex3f(+1, 0,-1);
  glVertex3f(+1,+1,-1);
  glVertex3f(+1,+1,+1);
  //  Left
  glNormal3f(-1, 0, 0);
  glVertex3f(-1, 0,-1);
  glVertex3f(-1, 0,+1);
  glVertex3f(-1,+1,+1);
  glVertex3f(-1,+1,-1);
  //  Top
  glNormal3f( 0,+1, 0);
  glVertex3f(-1,+1,+1);
  glVertex3f(+1,+1,+1);
  glVertex3f(+1,+1,-1);
  glVertex3f(-1,+1,-1);
  //  Bottom
  glNormal3f( 0,-1, 0);
  glVertex3f(-1, 0,-1);
  glVertex3f(+1, 0,-1);
  glVertex3f(+1, 0,+1);
  glVertex3f(-1, 0,+1);
  //  End
  glEnd();
  //  Undo transformations
  glPopMatrix();
}

void Roof(double x, double y, double z, double width, double height, double depth, double angle, double ax, double ay, double az) {
  //  Save transformation
  glPushMatrix();
  //  Offset, scale
  glTranslatef(x, y, z);
  glRotatef(angle, ax, ay, az);
  glScalef(width, height, depth);

  glColor3f(1, 0, 0);
  //  Front
  glBegin(GL_TRIANGLES);
  glNormal3f(-0.6,0.3,0);
  glVertex3f(-1, -1, +1);
  glVertex3f(-1, -1, -1);
  glVertex3f( 0, +1, 0);
  glEnd();
  //  Back
  glBegin(GL_TRIANGLES);
  glNormal3f(0,0.3,-0.6);
  glVertex3f(+1, -1, -1);
  glVertex3f(-1, -1, -1);
  glVertex3f( 0, +1, 0);
  glEnd();
  //  Right
  glBegin(GL_TRIANGLES);
  glNormal3f(0,-0.3,0.6);
  glVertex3f(+1, -1, +1);
  glVertex3f(-1, -1, +1);
  glVertex3f( 0, +1, 0);
  glEnd();
  //  Left
  glBegin(GL_TRIANGLES);
  glNormal3f(0.6,-0.3,0);
  glVertex3f(+1, -1, +1);
  glVertex3f(+1, -1, -1);
  glVertex3f( 0, +1, 0);
  glEnd();
  //  Bottom
  glBegin(GL_QUADS);
  glNormal3f( 0, -1,  0);
  glVertex3f(-1, -1, -1);
  glVertex3f(+1, -1, -1);
  glVertex3f(+1, -1, +1);
  glVertex3f(-1, -1, +1);
  glEnd();

  //  Undo transformations
  glPopMatrix();
}



/*
 *  Draw a house at (x,y,z)
 *  with height (h), length (l), width (w)
 *  at angle (angle) on x (ax), y (ay), or z (az)
 *
 */


void House (double x, double y, double z, double width, double height, double length, double angle, double ax, double ay, double az) {
  Cube(x, y, z, width, height, length, angle, ax, ay, az);
  Roof(x, y+height*1.5, z, width+0.1, height/2, length+0.1, angle, ax, ay, az);
}

void Tree (double x, double y, double z, double radius, double height) {
  Cylinder(x, y, z, radius/5, height);
  Cone(x, y+height/2, z, radius, height);
}

void Town () {

  for (int i=-2; i<3; i++) {
    if (i == 0)
      continue;
    double dist = 0.4 * i;
    House(0, 0, 0, 0.3, 0.5, 0.4, 0, 0, 0, 0);
    House(-dist*3, 0, -dist*4, 0.2, 1, 0.3, 45, 0, 1, 0);
    House(dist*4, 0, 0, 0.5, 0.4, 0.6, 30, 0, 1, 0);
    House(dist*1.5, 0, -dist*4, 0.2, 1, 0.3, 20, 0, 1, 0);
    House(-dist*4, 0, dist*2, 0.3, 0.7, 0.4, 80, 0, 1, 0);

    Tree(dist, 0, dist, 0.1, 0.2);
    Tree(dist*1.5, 0, -dist, 0.1, 0.4);
    Tree(-dist*2, 0, dist*2, 0.2, 0.6);
    Tree(dist, 0, -dist*2, 0.2, 0.5);
    Tree(dist*2, 0, -dist, 0.1, 0.2);
    Tree(-dist*1.5, 0, dist, 0.1, 0.4);
    Tree(dist, 0, -dist*0.4, 0.2, 0.6);
    Tree(dist, 0, dist*2, 0.2, 0.5);
    Tree(dist*4, 0, -dist*2, 0.1, 0.2);
    Tree(-dist*1.5, 0, dist*4, 0.1, 0.4);
    Tree(dist*3, 0, -dist*0.4, 0.2, 0.6);
    Tree(dist, 0, dist*3, 0.2, 0.5);
  }
}

/*
 *  Set projection
 */
void projection() {
   //  Tell OpenGL we want to manipulate the projection matrix
   glMatrixMode(GL_PROJECTION);
   //  Undo previous transformations
   glLoadIdentity();
   //  Perspective transformation
   if (mode)
      gluPerspective(fov,asp,dim/4,4*dim);
   //  Orthogonal projection
   else
      glOrtho(-asp*dim,+asp*dim, -dim,+dim, -dim,+dim);
   //  Switch to manipulating the model matrix
   glMatrixMode(GL_MODELVIEW);
   //  Undo previous transformations
   glLoadIdentity();
}


/*
 *  OpenGL (GLUT) calls this routine to display the scene
 */
void display () {
  // Erase the window and the depth buffer
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  // Enable Z-buffering in OpenGL
  glEnable(GL_DEPTH_TEST);
  // Undo previous transformations
  glLoadIdentity();

  //  Perspective - set eye position
  if (mode == 1){
    double Ex = -2*dim*Sin(th)*Cos(ph);
    double Ey = +2*dim        *Sin(ph);
    double Ez = +2*dim*Cos(th)*Cos(ph);
    gluLookAt(Ex,Ey,Ez , 0,0,0 , 0,Cos(ph),0);
  }
  //  Orthogonal - set world orientation
  else if(mode == 0){
    glRotatef(ph,1,0,0);
    glRotatef(th,0,1,0);
  }
  // First person view
  else if(mode == 2){
    // Recalculate where the camera is looking
    AX = -2*dim*Sin(th)*Cos(ph);
    AY = -2*dim*Sin(ph);
    AZ = -2*dim*Cos(th)*Cos(ph);
    // Orient the scene so it imitates first person movement
    gluLookAt(EX, EY, EZ, AX + EX, AY + EY, AZ + EZ, 0, 1, 0);
  }

  if(light){
    //  Translate intensity to color vectors
    float Ambient[] = {0.01*ambient ,0.01*ambient ,0.01*ambient ,1.0};
    float Diffuse[] = {0.01*diffuse ,0.01*diffuse ,0.01*diffuse ,1.0};
    float Specular[] = {0.01*specular,0.01*specular,0.01*specular,1.0};
    //  Light position
    float Position[]  = {light_distance*Cos(light_angle), light_height, light_distance*Sin(light_angle),1.0};

    //  Draw light position as ball (still no lighting here)
    glColor3f(1,1,1);
    Sun(Position[0], Position[1], Position[2], 0.3);
    //  OpenGL should normalize normal vectors
    glEnable(GL_NORMALIZE);
    //  Enable lighting
    glEnable(GL_LIGHTING);
    //  glColor sets ambient and diffuse color materials
    glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);
    //  Enable light 0
    glEnable(GL_LIGHT0);
    //  Set ambient, diffuse, specular components and position of light 0
    glLightfv(GL_LIGHT0,GL_AMBIENT ,Ambient);
    glLightfv(GL_LIGHT0,GL_DIFFUSE ,Diffuse);
    glLightfv(GL_LIGHT0,GL_SPECULAR,Specular);
    glLightfv(GL_LIGHT0,GL_POSITION,Position);
  }else{
    glDisable(GL_LIGHTING);
  }

  glPushMatrix();
  glColor3f(0, 0, 1);
  Town();
  glPopMatrix();

  // Draw axes (white)
  glDisable(GL_LIGHTING);
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
  Print("Angle=%d,%d  Axes=%s  Dim=%.1f  Projection=%s  LightHeight=%d  LightMove=%s", th, ph, axes ? "On" : "Off", dim, views[mode], light_height, light_move ? "Move" : "Stop");

  // Render the scene and make it visible
  glFlush();
  glutSwapBuffers();
}


/*
 *  Incrment current line trace
 */
void idle () {
  double t = glutGet(GLUT_ELAPSED_TIME)/1000.0;
  light_angle = fmod(90*t,360);
  glutPostRedisplay();
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
  projection();
  glutPostRedisplay();
}

/*
 *  GLUT calls this routine when a key is pressed
 */
void key(unsigned char ch, int x, int y) {
  // Exit on ESC
  if (ch == 27) {
    exit(0);
  }
  // Reset view angle
  else if (ch == '0') {
    th = ph = 0;
    axes = 1;
    th = ph = 0;
    fov = 55;
    dim = 5;
    light_move = 1;
    light_distance = 5;
    light_height = 3;
  }
  // Toggle axes
  else if (ch == 'a' || ch == 'A') {
    axes = 1-axes;
  }
  else if (ch == 'm' || ch == 'M') {
    mode = (mode + 1) % 3;
  }
  else if (ch == '+') {
    dim -= 0.1;
  }
  else if (ch == '-') {
    dim += 0.1;
  }
  // Move forward in the scene
  else if(ch == 'f' || ch == 'F') {
    EX += AX*.1;
    EZ += AZ*.1;
  }
  // Move backwards in the scene
  else if(ch == 'b' || ch == 'B') {
    EX -= AX*.1;
    EZ -= AZ*.1;
  }
  // Look down x-axis
  else if (ch == 'x') {
    th = -90;
    ph = 0;
  }
  // Look down y-axis
  else if (ch == 'y') {
    th = 0;
    ph = 90;
  }
  // Look down z-axis
  else if (ch == 'z') {
    th = ph = 0;
  }
  // Increase light height
  else if (ch == 'u' || ch == 'U') {
    light_height++;
  }
  // Decrease light height
  else if (ch == 'd' || ch == 'D') {
    light_height--;
  }
  // Increase light distance
  else if (ch == 'j' || ch == 'J') {
    light_distance++;
  }
  // Decrease light distance
  else if (ch == 'k' || ch == 'K') {
    if (light_distance > 0)
      light_distance--;
  }
  // Togle light move
  else if (ch == 's' || ch == 'S') {
    light_move = 1-light_move;
  }

  projection();

  if (light_move) {
    glutIdleFunc(idle);
  } else {
    glutIdleFunc(NULL);
  }


  glutPostRedisplay();
}

/*
 *  GLUT calls this routine when the window is resized
 */
void reshape(int width,int height) {
  //  Ratio of the width to the height of the window
  asp = (height>0) ? (double)width/height : 1;
  //  Set the viewport to the entire window
  glViewport(0,0, width,height);
  //  Tell OpenGL we want to manipulate the projection matrix
  glMatrixMode(GL_PROJECTION);
  //  Undo previous transformations
  glLoadIdentity();
  //  Switch to manipulating the model matrix
  glMatrixMode(GL_MODELVIEW);
  //  Undo previous transformations
  glLoadIdentity();
  //  Set the viewport to the entire window
  glViewport(0,0, width,height);
  //  Set projection
  projection();
}

/*
 *  Start up GLUT and tell it what to do
 */
int main (int argc, char *argv[]) {
  // Initialize GLUT and process user parameters
  glutInit(&argc,argv);
  // Request double buffered, true color window with Z buffering
  glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
  // Request 800 x 800 pixel window
  glutInitWindowSize(800, 800);
  // Create the window
  glutCreateWindow("Lighting - Ryo Suzuki");
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
