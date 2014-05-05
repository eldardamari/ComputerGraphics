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

double MAX_HEIGHT = 5.5;

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

void drawCube(double x1, double y1, double z1, double x2, double y2, double z2, double h)
{
      glColor3f( 1.0,  1.0, 1.0 );

      //side - FRONT
      glBegin(GL_QUADS);
      glVertex3f( x1, y1, z1 );      // P1 is red
      glVertex3f( x1, y1, h  );      // P2 is green
      glVertex3f( x2, y1, h  );      // P3 is blue
      glVertex3f( x2, y1, z1 );      // P4 is purple

      glEnd();

      //side - BACK
      glBegin(GL_QUADS);
      glVertex3f( x1, y2, z2 );
      glVertex3f( x1, y2, h );
      glVertex3f( x2, y2, h );
      glVertex3f( x2, y2, z2 );
      glEnd();

      //side - RIGHT
      glBegin(GL_QUADS);
      glVertex3f( x2, y1, z1 );
      glVertex3f( x2, y1, h );
      glVertex3f( x2, y2, h );
      glVertex3f( x2, y2, z2 );
      glEnd();

      //side - LEFT
      glBegin(GL_QUADS);
      glVertex3f( x1, y1, z1 );
      glVertex3f( x1, y1, h );
      glVertex3f( x1, y2, h );
      glVertex3f( x1, y2, z2 );
      glEnd();

      //side - TOP
      glBegin(GL_QUADS);
      glVertex3f( x1, y1, h );
      glVertex3f( x1, y2, h );
      glVertex3f( x2, y2, h );
      glVertex3f( x2, y1, h );
      glEnd();

      //side - BOTTOM
      glBegin(GL_QUADS);
      glVertex3f( x1, y1, z1 );
      glVertex3f( x1, y2, z1 );
      glVertex3f( x2, y2, z2 );
      glVertex3f( x2, y1, z2 );
      glEnd();
}

void drawPyramid(double x1, double y1, double z1, double x2, double y2, double z2, double centerX, double centerY, double centerZ )
{
    glColor3f (   1.0,    0.0,    0.0 );

    // ROOF - Right
    glBegin(GL_TRIANGLES);
    glVertex3f( x2, y1, z1 );
    glVertex3f( centerX, centerY, centerZ );
    glVertex3f( x2, y2, z2 );
    glEnd();

    // ROOF - Back
    glBegin(GL_TRIANGLES);
    glVertex3f(   x1,    y2,   z2   );
    glVertex3f( centerX, centerY, centerZ );
    glVertex3f(     x2,    y2,   z1   );
    glEnd();

    // ROOF - Left
    glBegin(GL_TRIANGLES);
    glVertex3f( x1, y1, z1 );
    glVertex3f( centerX, centerY, centerZ );
    glVertex3f( x1, y2, z1 );
    glEnd();

    // ROOF - Front
    glBegin(GL_TRIANGLES);
    glVertex3f( x1, y1, z1 );
    glVertex3f( centerX, centerY, centerZ );
    glVertex3f( x2, y1, z2 );
    glEnd();
}

void drawHouse(double x1, double y1, double z1, double x2, double y2, double z2)
{
    double normalizedZ1 = (z1 * MAX_HEIGHT)/255;
    double normalizedZ2 = (z2 * MAX_HEIGHT)/255;
    drawCube(x1, y1, normalizedZ1, x2, y2, normalizedZ2, 3.5);

    double centerX = x1 + ((x2 - (x1)) / 2);
    double centerY = y1 + ((y2 - (y1)) / 2);
    double centerZ = 5;
    // ROOF
    drawPyramid(x1, -2.5, 3.5, x2, -5.5, 3.5, centerX, centerY, centerZ );

    // DOOR
    glColor3f (   0.5,    0.5,    0.5 );
    glBegin(GL_QUADS);
    glVertex3f( centerX+0.5, y1, normalizedZ1 );
    glVertex3f( centerX+0.5, y1, normalizedZ1 + 1.5 );
    glVertex3f( centerX-0.5, y1, normalizedZ2 + 1.5 );
    glVertex3f( centerX-0.5, y1, normalizedZ2 );
    glEnd();

    // WINDOW - Right
    glColor3f (   0.0,    1.0,    0.0 );
    glBegin(GL_QUADS);
    glVertex3f( x2, centerY+1, normalizedZ1 + 1.5 );
    glVertex3f( x2, centerY+1, normalizedZ1 + 2.5 );
    glVertex3f( x2, centerY-1, normalizedZ2 + 2.5 );
    glVertex3f( x2, centerY-1, normalizedZ2 +1.5 );
    glEnd();

    // WINDOW - Left
    glColor3f (   0.0,    1.0,    0.0 );
    glBegin(GL_QUADS);
    glVertex3f( x1, centerY+1, normalizedZ1 + 1.5 );
    glVertex3f( x1, centerY+1, normalizedZ1 + 2.5 );
    glVertex3f( x1, centerY-1, normalizedZ2 + 2.5 );
    glVertex3f( x1, centerY-1, normalizedZ2 +1.5 );
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

    drawHouse(2.5,-2.5,7,5,-5.5,7);

  glutSwapBuffers();

}

int main(int argc, char**argv)
{
    glutInit(&argc, argv);
	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

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

