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

	gluPerspective(45, 1, 1.0, 200);
	gluLookAt(0, 0, 50, 0, 0, 0, 0, 0, 0);  //define view direction

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

    float XP[3] = {100,0,0}, XN[3] = {-100,0,0},
          YP[3] = {0,100,0}, YN[3] = {0,-100,0},
          ZP[3] = {0,0,100}, ZN[3] = {0,0,-100};

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
    float   houseLeftBackCorner[3] = {2.5,-2.5,7},
            houseRightBackCorner[3] = {2.5,-5.5,7},
            houseRightFrontCorner[3] = {5,-5.5,7},
            houseLeftFrontCorner[3] = {5,-2.5,7};

    glBegin(GL_QUADS);

    glColor3f (1,1,1); // +Z axis is blue.
    glVertex3fv (houseLeftBackCorner);
    glVertex3fv (houseRightBackCorner);
    glVertex3fv (houseRightFrontCorner);
    glVertex3fv (houseLeftFrontCorner);

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
    glLoadIdentity();

//    gluPerspective(45.0,                  //The camera angle
//                   0.0 / 1.2,             //The width-to-height ratio
//                   300.0,                   //The near z clipping coordinate
//                   900.0);                //The far z clipping coordinate

    // Rotate when user changes rotate_x and rotate_y
    glRotatef( rotate_x, 1.0, 0.0, 0.0 );
    glRotatef( rotate_y, 0.0, 1.0, 0.0 );
    glRotatef( rotate_z, 0.0, 1.0, 1.0 );

    drawHouse();
    draw_axes();


}

int main(int argc, char**argv) {

    glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutInitWindowSize(900, 900);
	glutCreateWindow("Assignment 2");

    //  Enable Z-buffer depth test
	glEnable(GL_DEPTH_TEST);

    // Callback functions
	glutDisplayFunc(mydisplay);
//    glutReshapeFunc(changeSize);
//	glutIdleFunc(mydisplay);
	glutSpecialFunc(specialKeys);

    //  Pass control to GLUT for events
    glutMainLoop();

    return 0;
}

