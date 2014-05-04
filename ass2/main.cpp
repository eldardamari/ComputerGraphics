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
#define GL_PI 3.1415926535897932384626433832795


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

void draw_wheel(GLfloat Cx, GLfloat Cy, GLfloat Cz)
{
    GLfloat z, y,angle;
    GLfloat fPlate[36][3];
    GLfloat bPlate[36][3];
    int i;
    z = 0.0f;
    y = 0.0f;

    glBegin(GL_TRIANGLE_FAN);
    glColor3f(1,0,0);

    glVertex3f(Cx + 0.5f,Cy,Cz);
    for (i = 0, angle = 0.0f ; angle < 360.0f ; angle += 10.0f, i++) {
        z = Cz*sin(angle*GL_PI/180.0f) + Cz;
        y = Cz*cos(angle*GL_PI/180.0f);
        glVertex3f(Cx + 0.5f, y , z);
        fPlate[i][0] = Cx + 0.5f;
        fPlate[i][1] = y;
        fPlate[i][2] = z;
    }
    z = Cz*sin(0.0f) + Cz;
    y = Cz*cos(0.0f);
    glVertex3f(Cx + 0.5f, y , z);

    glEnd();

    /*glBegin(GL_TRIANGLES);
    glColor3f(1,0,0);

    glVertex3f(Cx + 0.5,Cy,Cz);
    glVertex3f(Cx + 0.5, y , z);
    y = Cz*cos(0.0f);
    z = Cz*sin(0.0f) + Cz;
    glVertex3f(Cx + 0.5, y , z);

    glEnd();*/
    
    glBegin(GL_TRIANGLE_FAN);
    glColor3f(0,1,0);

    glVertex3f(Cx + -0.5f,Cy,Cz);
    for (i=0,angle = 0.0f ; angle < 360 ; angle += 10.0f,i++) {
        z = Cz*sin(angle*GL_PI/180.0f) + Cz;
        y = Cz*cos(angle*GL_PI/180.0f);
        glVertex3f(Cx + -0.5f, y , z);
        bPlate[i][0] = Cx + -0.5f;
        bPlate[i][1] = y;
        bPlate[i][2] = z;
    }
    z = Cz*sin(0.0f) + Cz;
    y = Cz*cos(0.0f);
    glVertex3f(Cx + -0.5f, y , z);
    glEnd();
    
    /*glBegin(GL_TRIANGLES);
    glColor3f(0,1,0);

    glVertex3f(Cx + -0.5,Cy,Cz);
    glVertex3f(-0.5, y , z);
    y = Cz*cos(0.0f);
    z = Cz*sin(0.0f) + Cz;
    glVertex3f(-0.5, y , z);
    glEnd();*/

    // cover wheel
    glBegin(GL_TRIANGLE_STRIP);
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
    glLoadIdentity();

    // Rotate when user changes rotate_x and rotate_y
    glRotatef( rotate_x, 1.0, 0.0, 0.0 );
    glRotatef( rotate_y, 0.0, 1.0, 0.0 );
    glRotatef( rotate_z, 0.0, 1.0, 1.0 );

    /*drawHouse();*/
    draw_axes();
    draw_wheel(0.0f,0.0f,3.0f);
    draw_wheel(3.0f,0.0f,3.0f);
    draw_wheel(0.0f,5.0f,3.0f);
    draw_wheel(3.0f,5.0f,3.0f);
}

int main(int argc, char**argv) {

    glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);

	glutInitWindowSize(900, 900);
	glutCreateWindow("Assignment 2");

    //  Enable Z-buffer depth test
	glEnable(GL_DEPTH_TEST);

    // Callback functions
	glutDisplayFunc(mydisplay);
    init();
//    glutReshapeFunc(changeSize);
//	glutIdleFunc(mydisplay);
	glutSpecialFunc(specialKeys);

    //  Pass control to GLUT for events
    glutMainLoop();

    return 0;
}
