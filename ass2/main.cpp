#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <math.h>
#include <stdlib.h>

#ifdef __APPLE__
    #include <GLUT/glut.h>
#else 
    #include <GL/glut.h>
#endif
/*#define GL_PI 3.1415926535897932384626433832795*/


using namespace std;

double rotate_y = 0;
double rotate_x = 0;
double rotate_z = 0;

void init()
{
	float modelMatrix[16], projectionMatrix[16];
	glClearColor(1, 1, 1, 1);

	glMatrixMode(GL_PROJECTION); /* switch matrix mode */
	glLoadIdentity();			 //load Identity matrix

	gluPerspective(45, 1, 2, 25);
	gluLookAt(20,9, 5, 0.7,0.5,1, 0, 0, 1);  //define view direction
	
	//glOrtho(-10.0, 10.0, -10.0, 10.0, -10.0, 10.0); //For DEBUG

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

    glLineWidth (2.0);
    glBegin (GL_LINES);

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

    /*glutSwapBuffers();*/
    glFlush();

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
    rotate_z += 10;
  //  Left arrow - decrease rotation by 5 degree
  else if (key == GLUT_KEY_LEFT)
    rotate_z -= 10;
  else if (key == GLUT_KEY_UP)
    rotate_x += 10;
  else if (key == GLUT_KEY_DOWN)
    rotate_x -= 10;
  //  Request display update
  glutPostRedisplay();
}

void draw_wheel(GLfloat Cx, GLfloat Cy, GLfloat Cz)
{
    GLfloat z, y,angle;
    GLfloat fPlate[36][3];
    GLfloat bPlate[36][3];
    GLfloat GL_PI  = 3.1415926535897932384626433832795;
    int i;
    z = 0.0f;
    y = 0.0f;

    glBegin(GL_TRIANGLE_FAN);
    glColor3f(1,0,0);
    Cx += 0.5;

    glVertex3f(Cx,Cy,Cz);
    for (i = 0, angle = 0.0f ; angle < 360.0f ; angle += 10.0f, i++) {
        z = 3.0f*(GLfloat)sin(angle*GL_PI/180.0f) + Cz;
        y = 3.0f*(GLfloat)cos(angle*GL_PI/180.0f) + Cy;
        glVertex3f(Cx, y , z);
        fPlate[i][0] = Cx;
        fPlate[i][1] = y;
        fPlate[i][2] = z;
    }
    z = 3.0f*(GLfloat)sin(0.0f) + Cz;
    y = 3.0f*(GLfloat)cos(0.0f) + Cy;
    glVertex3f(Cx , y , z);

    glEnd();
    glFlush();

    glBegin(GL_TRIANGLE_FAN);
    glColor3f(0,1,0);
    Cx -= 0.5;

    glVertex3f(Cx,Cy,Cz);
    for (i=0,angle = 0.0f ; angle < 360 ; angle += 10.0f,i++) {
        z = 3.0f*(GLfloat)sin(angle*GL_PI/180.0f) + Cz;
        y = 3.0f*(GLfloat)cos(angle*GL_PI/180.0f) + Cy;
        glVertex3f(Cx, y , z);
        bPlate[i][0] = Cx;
        bPlate[i][1] = y;
        bPlate[i][2] = z;
    }
    z = 3.0f*(GLfloat)sin(0.0f) + Cz;
    y = 3.0f*(GLfloat)cos(0.0f) + Cy;
    glVertex3f(Cx, y , z);
    glEnd();
    
    glFlush();
    
    // cover wheel
    glBegin(GL_TRIANGLE_STRIP);
    glColor3f(0,0,1);
    for (i = 0 ; i < 36 ; i++) {
        glVertex3f( fPlate[i][0], fPlate[i][1], fPlate[i][2]); 
        glVertex3f( bPlate[i][0], bPlate[i][1], bPlate[i][2]); 
    }
        glVertex3f( fPlate[0][0], fPlate[0][1], fPlate[0][2]); 
        glVertex3f( bPlate[0][0], bPlate[0][1], bPlate[0][2]);
    glEnd();

    glFlush();
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
    draw_wheel(0.0f,0.0f,3.0f);
    draw_wheel(3.0f,0.0f,3.0f);
    draw_wheel(0.0f,10.0f,3.0f);
    draw_wheel(3.0f,10.0f,3.0f);
}

int main(int argc, char**argv) {

    glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);

	glutInitWindowSize(900, 900);
	glutCreateWindow("Assignment 2");

    //  Enable Z-buffer depth test
	glEnable(GL_DEPTH_TEST);

	glClearColor(1, 1, 1, 1);
	gluPerspective(60, 1, 15, 100);
    gluLookAt(0, 0, 30, 0, 0, 0, 0, 1, 0);  //define view direction

    // Callback functions
	glutDisplayFunc(mydisplay);
    //init();
//    glutReshapeFunc(changeSize);
//	glutIdleFunc(mydisplay);
	glutSpecialFunc(specialKeys);

    //  Pass control to GLUT for events
    glutMainLoop();

    return 0;
}
