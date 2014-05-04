#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <stdlib.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

using namespace std;

double rotate_y = 0;
double rotate_x = 0;
double rotate_z = 0;

void init()
{
	float modelMatrix[16], projectionMatrix[16];
	glClearColor(0, 0, 0, 1);

	glMatrixMode(GL_PROJECTION); /* switch matrix mode */
	glLoadIdentity();			 //load Identity matrix

	gluPerspective(60, 1, 1, 100);
	gluLookAt(0, 0, 15, 0, 0, 0, 0, 1, 0);  //define view direction

//	glOrtho(-10.0, 10.0, -10.0, 10.0, -10.0, 10.0); //For DEBUG

	/* get matrices */
	glGetFloatv(GL_PROJECTION_MATRIX, projectionMatrix);
	glGetFloatv(GL_MODELVIEW_MATRIX, modelMatrix);

	/* return to modelview mode */
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void draw_axes(void)
{
    float ORG[3] = {0,0,0};

    float XP[3] = {10,0,0}, XN[3] = {-10,0,0},
          YP[3] = {0,10,0}, YN[3] = {0,-10,0},
          ZP[3] = {0,0,10}, ZN[3] = {0,0,-10};

    glBegin (GL_LINES);
    glLineWidth (2.0);

    glColor3f (1,0,0); // X axis is red.
    glVertex3fv (XN);
    glVertex3fv (XP);

    glColor3f (0,1,0); // Y axis is green.
    glVertex3fv (YN);
    glVertex3fv (YP);


    glColor3f (0,0,1); // Z axis is blue.
    glVertex3fv (ZN);
    glVertex3fv (ZP);

    glEnd();

    glutSwapBuffers();
}

void drawHouse(void)
{
    //Multi-colored side - FRONT
      glBegin(GL_POLYGON);

      glColor3f( 1.0, 0.0, 0.0 );     glVertex3f(  0.5, -0.5, -0.5 );      // P1 is red
      glColor3f( 0.0, 1.0, 0.0 );     glVertex3f(  0.5,  0.5, -0.5 );      // P2 is green
      glColor3f( 0.0, 0.0, 1.0 );     glVertex3f( -0.5,  0.5, -0.5 );      // P3 is blue
      glColor3f( 1.0, 0.0, 1.0 );     glVertex3f( -0.5, -0.5, -0.5 );      // P4 is purple

      glEnd();

      // White side - BACK
      glBegin(GL_POLYGON);
      glColor3f(   1.0,  1.0, 1.0 );
      glVertex3f(   5,      -5.5,   0   );
      glVertex3f(   5,      -5.5,   7   );
      glVertex3f(   5,      -5.5,   7   );
      glVertex3f(   5,      -5.5,   0   );
      glEnd();

      // Purple side - RIGHT
      glBegin(GL_POLYGON);
      glColor3f(  1.0,  0.0,  1.0 );
      glVertex3f(   5,      -2.5,   0   );
      glVertex3f(   5,      -5.5,   0   );
      glVertex3f(   5,      -5.5,   7   );
      glVertex3f(   5,      -2.5,   7   );
      glEnd();

      // Green side - LEFT
      glBegin(GL_POLYGON);
      glColor3f(   0.0,  1.0,  0.0 );
      glVertex3f(   2.5,    -2.5,   0   );
      glVertex3f(   2.5,    -5.5,   0   );
      glVertex3f(   2.5,    -5.5,   7   );
      glVertex3f(   2.5,    -2.5,   7   );
      glEnd();

      // Blue side - TOP
      glBegin(GL_POLYGON);
      glColor3f(   0.0,  0.0,  1.0 );
      glVertex3f(   2.5,    -2.5,   7   );
      glVertex3f(   2.5,    -5.5,   7   );
      glVertex3f(   5,      -5.5,   7   );
      glVertex3f(   5,      -2.5,   7   );
      glEnd();

      // Red side - BOTTOM
      glBegin(GL_QUADS);
      glColor3f (   1.0,    0.0,    0.0 );
      glVertex3f(   2.5,    -2.5,   0   );
      glVertex3f(   2.5,    -5.5,   0   );
      glVertex3f(   5,      -5.5,   0   );
      glVertex3f(   5,      -2.5,   0   );
      glEnd();

      glutSwapBuffers();

}

// ----------------------------------------------------------
// specialKeys() Callback Function
// ----------------------------------------------------------
void specialKeys( int key, int x, int y ) {

  //  Right arrow - increase rotation by 5 degree
  if (key == GLUT_KEY_RIGHT)
    rotate_z += 5;

  //  Left arrow - decrease rotation by 5 degree
  else if (key == GLUT_KEY_LEFT)
    rotate_z -= 5;

  else if (key == GLUT_KEY_UP)
    rotate_x += 5;

  else if (key == GLUT_KEY_DOWN)
    rotate_x -= 5;

  //  Request display update
  glutPostRedisplay();
}

void mydisplay(void)
{
    //  Clear screen and Z-buffer
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    // Reset transformations
    glMatrixMode(GL_PROJECTION); /* switch matrix mode */
    glLoadIdentity();

    // Rotate when user changes rotate_x and rotate_y
    glRotatef( rotate_x, 1.0, 0.0, 0.0 );
    glRotatef( rotate_z, 0.0, 0.0, 1.0 );

    glMatrixMode(GL_MODELVIEW); /* switch matrix mode */

    draw_axes();

    drawHouse();

  glutSwapBuffers();

}

int main(int argc, char**argv) {

    glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutInitWindowSize(900, 900);
	glutCreateWindow("Assignment 2");

    //  Enable Z-buffer depth test
	glEnable(GL_DEPTH_TEST);

	gluPerspective(60, 1, 15, 100);
    gluLookAt(0, 0, 25, 0, 0, 0, 0, 1, 0);  //define view direction

    // Callback functions
	glutDisplayFunc(mydisplay);
	glutSpecialFunc(specialKeys);

    //  Pass control to GLUT for events
    glutMainLoop();

    return 0;
}

