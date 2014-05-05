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

#define max_hight 5.5
using namespace std;

double rotate_y = 0;
double rotate_x = 0;
double rotate_z = 0;
GLfloat zoomFactor = 1.0f;

double MAX_HEIGHT = 5.5;

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

void drawCube(double x1, double y1, double z1, double x2, double y2, double z2, double h)
{
      glColor3f( 0.0,  0.0, 0.0 );

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
//    glBegin(GL_QUADS);
//    glVertex3f( x1, y1, z1 );
//    glVertex3f( x1, y2, z1 );
//    glVertex3f( x2, y2, z2 );
//    glVertex3f( x2, y1, z2 );
//    glEnd();

    // WINDOW

    glutSwapBuffers();
}

// ----------------------------------------------------------
// zoomFactoospecialKeys() Callback Function
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
  else if (key == GLUT_KEY_F2)
    zoomFactor += 10;
  else if (key == GLUT_KEY_F3)
   zoomFactor  -= 10;
  //  Request display update
  glutPostRedisplay();
}
void setProjectionMatrix (int width, int height)
{
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluPerspective (50.0*zoomFactor, (float)width/(float)height, 1.0f, 1.0f);
   /* ...Where 'zNear' and 'zFar' are up to you to fill in. */
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
    GLfloat rCz = Cz;
    /*Cz = Cz*max_hight/255.0f;*/


    glBegin(GL_TRIANGLE_FAN);
    glColor3f(1,0,0);
    Cx += 0.2;

    glVertex3f(Cx,Cy,Cz);
    for (i = 0, angle = 0.0f ; angle < 360.0f ; angle += 10.0f, i++) {
        z = rCz*(GLfloat)sin(angle*GL_PI/180.0f) + Cz;
        y = rCz*(GLfloat)cos(angle*GL_PI/180.0f) + Cy;
        glVertex3f(Cx, y , z);
        fPlate[i][0] = Cx;
        fPlate[i][1] = y;
        fPlate[i][2] = z;
    }
    z = rCz*(GLfloat)sin(0.0f) + Cz;
    y = rCz*(GLfloat)cos(0.0f) + Cy;
    glVertex3f(Cx , y , z);

    glEnd();
    glFlush();

    glBegin(GL_TRIANGLE_FAN);
    glColor3f(0,1,0);
    Cx -= 0.2;

    glVertex3f(Cx,Cy,Cz);
    for (i=0,angle = 0.0f ; angle < 360 ; angle += 10.0f,i++) {
        z = rCz*(GLfloat)sin(angle*GL_PI/180.0f) + Cz;
        y = rCz*(GLfloat)cos(angle*GL_PI/180.0f) + Cy;
        glVertex3f(Cx, y , z);
        bPlate[i][0] = Cx;
        bPlate[i][1] = y;
        bPlate[i][2] = z;
    }
    z = rCz*(GLfloat)sin(0.0f) + Cz;
    y = rCz*(GLfloat)cos(0.0f) + Cy;
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

void drawWindShield()
{
    glColor3f( 1.0,  0.0, 0.0 );
    glBegin(GL_QUADS);
        glVertex3f( 1.0, 4.0f, 1.5f );
        glVertex3f( 0.2f, 4.0f, 1.5f );
        glVertex3f( 0.2f, 4.0f, 3.5f);
        glVertex3f( 1.0f,4.0f, 3.5f );
    glEnd();
    glFlush();
    
    glColor3f( 1.0,  0.0, 1.0 );
    glBegin(GL_QUADS);
        glVertex3f( 0.2f, 0.0f, 1.5f );
        glVertex3f( 1.0f, 0.0f, 1.5f );
        glVertex3f( 1.0f, 0.0f, 3.5f);
        glVertex3f( 0.2f,0.0f, 3.5f );
    glEnd();
    glFlush();


}
void drawCarBody(void)
{
    drawCube(0.2f,-1.2f,1.0f, 1.0f, 6.2f,1.0f,2.5f);
    drawCube(0.2f,0.0f,3.5f, 1.0f, 4.0f,3.5f,2.0f);
    drawWindShield();
}

void drawCar(void)
{
    draw_wheel(0.0f ,0.0f   ,1.0f);
    draw_wheel(1.0f ,0.0f   ,1.0f);
    draw_wheel(0.0f ,5.0f  ,1.0f);
    draw_wheel(1.0f ,5.0f  ,1.0f);

    drawCarBody();


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
    drawCar();

    drawHouse(2.5,-2.5,7,5,-5.5,7);

    glutSwapBuffers();

}

int main(int argc, char**argv) {

    glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);

	glutInitWindowSize(900, 900);
	glutCreateWindow("Assignment 2");

    //  Enable Z-buffer depth test
	glEnable(GL_DEPTH_TEST);

	glClearColor(1, 1, 1, 1);
	gluPerspective(45, 1, 15, 100);
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
