#include "CSCIx229.h"

/*
 *  GLUT calls this toutine when there is nothing else to do
 */
void idle()
{
   double t = glutGet(GLUT_ELAPSED_TIME)/1000.0;
   light_angle = fmod(90*t,360);
   glutPostRedisplay();
}
