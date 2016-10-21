#include "CSCIx229.h"


/*
 *  GLUT calls this routine when a key is pressed
 */
void key(unsigned char ch,int x,int y)
{
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

  Project(fov,asp,dim);

  if (light_move) {
    glutIdleFunc(idle);
  } else {
    glutIdleFunc(NULL);
  }
  glutPostRedisplay();
}
