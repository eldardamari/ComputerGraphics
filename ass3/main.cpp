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

GLfloat rotate_y = 0;
GLfloat rotate_x = 0;
GLfloat rotate_z = 0;

GLfloat zoom = 1;

GLfloat angle = 0;
GLfloat light = 0;

GLfloat light_ambient[3] =      { 0.5, 0.5, 0.5 };
GLfloat light_diffuse[3] =      { 0.0, 0.5, 0.5 };
GLfloat light_specular[3] =     { 0.0, 0.0, 0.5 };
GLfloat light_direction[3] =    {0, -1, 0 };
GLfloat shine[] =               { 5.0 };

void initLight(void)
{
    // Light
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, light_direction);

    glEnable(GL_LIGHT0);

    glMaterialfv(GL_FRONT, GL_SHININESS,shine);

    glEnable(GL_LIGHTING);
    glShadeModel(GL_SMOOTH);

    glEnable(GL_COLOR_MATERIAL);
//    glEnable(GL_NORMALIZE); // maintain normal vectors size
}

void init()
{
	glMatrixMode(GL_PROJECTION); /* switch matrix mode */
	glLoadIdentity();			 //load Identity matrix

    gluPerspective(60,1,2,100);

    gluLookAt(0,0, 20, 0,0,0, 0, 1, 0);  //define view direction
//    glRotatef(0,0,20,1);

    initLight();


	/* return to modelview mode */
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void specialKeys(int key,int x,int y)
{
    glMatrixMode(GL_PROJECTION);

    switch(key) {
        case GLUT_KEY_RIGHT:
            glRotatef( -4, 0.0, 0.0, 1.0 );
            break;
        case GLUT_KEY_LEFT:
            glRotatef( 4, 0.0, 0.0, 1.0 );
            break;
        case GLUT_KEY_UP:
            glRotatef( 4, 1.0, 0.0, 0.0 );
            break;
        case GLUT_KEY_DOWN:
            glRotatef( -4, 1.0, 0.0, 0.0 );
            break;
        case GLUT_KEY_F2:
            zoom = 1;
            zoom += 0.1;
            glScalef(zoom,zoom,zoom);
            break;
        case GLUT_KEY_F3:
            zoom = 1;
            zoom -= 0.1;
            glScalef(zoom,zoom,zoom);
            break;
    }

    glMatrixMode(GL_MODELVIEW);

    //  Request display update
    glutPostRedisplay();
}

void draw_axes(void)
{
    float ORG[3] = {0,0,0};

    float XP[3] = {10,0,0}, XN[3] = {-10,0,0},
          YP[3] = {0,10,0}, YN[3] = {0,-10,0},
          ZP[3] = {0,0,10}, ZN[3] = {0,0,-10};

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
}

void test(void)
{
    glLoadIdentity ();

    glutSwapBuffers();
}

void mydisplay(void)
{	
    // always make sure to be on MODEL_VIEW matric here!
    glLoadIdentity();

    //  Clear screen and Z-buffer
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    draw_axes();

    glutSwapBuffers();
}

int main(int argc, char**argv) 
{
    glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(900, 900);
	glutCreateWindow("Assignment 3");

    //  Enable Z-buffer depth test
	glEnable(GL_DEPTH_TEST);
    
    init();

    // Callback functions
	glutDisplayFunc(mydisplay);
//	glutIdleFunc(rotateSun);
    glutSpecialFunc(specialKeys);

    //  Pass control to GLUT for events
    glutMainLoop();

    return 0;
}