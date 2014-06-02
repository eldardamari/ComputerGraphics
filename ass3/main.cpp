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
GLfloat zoom = 100;

#define maxSize 65535
//float rot;
GLfloat vertices[maxSize*3];
GLfloat normals[maxSize*2*3];
GLuint faces[maxSize*4*3];
GLuint faceElements[maxSize*2];

int vIndex,vnIndex,fIndex,eIndex;
FILE *f;

int numberOfObjectsInScene;

GLfloat light_ambient[]     = { 0.5, 0.5, 0.5, 1.0 };
GLfloat light_diffuse[]     = { 0.0, 0.5, 0.5, 1.0 };
GLfloat light_specular[]    = { 0.0, 0.0, 0.5, 1.0 };
GLfloat light_position[]    = { 0, 1.0, 1,0 };
GLfloat light_direction[]   = { 0, -1, 0 };
GLfloat shine[]             = { 5.0 };

void readMaterials(GLfloat materials[]) {

    f = fopen("colorTable.csv", "r");

    int index = 0;
    float value[9];

    while ( fscanf(f, "%d,%f,%f,%f,%f,%f,%f,%f,%f,%f\n", &index, &value[0], &value[1], &value[2], &value[3], &value[4], &value[5], &value[6], &value[7], &value[8]) > 0)
    {
        for (int i = 0; i < 9;i++)
            materials[((index-1)*9)+i] = value[i];
    }

    fclose(f);
}

void meterials(void)
{
    GLfloat materials[numberOfObjectsInScene*9];

    readMaterials(materials);

//    for(int i = 0 ; i < sizeof(materials)/sizeof(*materials) ; i++)
//    {
//        cout << materials[i] << endl;
//    }

	GLfloat mat_a[] = {materials[0],materials[1],materials[2]};
	GLfloat mat_d[] = {materials[3],materials[4],materials[5]};
	GLfloat mat_s[] = {materials[6],materials[7],materials[8]};

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_a);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_d);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_s);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shine);
}

void init()
{
    glClearColor(0,0,0,1);  //black

    //  Enable Z-buffer depth test
	glEnable(GL_DEPTH_TEST);

    glEnable (GL_LIGHTING);
    glEnable (GL_LIGHT0);

    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, light_direction);

    meterials();

    glMatrixMode(GL_PROJECTION); /* switch matrix mode */
	glLoadIdentity();			 //load Identity matrix

    gluPerspective(60,1,2,100);
    gluLookAt(0,0,60,0,0,0,0,1,0);
//    glTranslatef(0,0,20);

	/* return to modelview mode */
	glMatrixMode(GL_MODELVIEW);
}

void reshape( int width, int height )
{
    glViewport(0, 0, (GLsizei)width, (GLsizei)height );
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    GLfloat ratio = (GLfloat)width / (GLfloat)height;
    gluPerspective(60, ratio, 2, 100);
	glTranslatef(0.0f, -50, -100.0f);
    glMatrixMode(GL_MODELVIEW);
}

void reader()
{
	char c;
	f = fopen("scene.obj","r");
	int b = 0;
	float tmp;
	vIndex = 0;
	vnIndex = 0;
	fIndex = 0;
	eIndex = 0;
	int line = 0;

	c = fgetc(f);

	while(  c!=EOF && vIndex  < maxSize*3    &&
	                  vnIndex < maxSize*2*3  &&
	                  fIndex  < maxSize*4*3  )
	{
		line++;
		switch(c)
		{
			/*
			case 'o':
				fscanf(f," %f\n",&tmp);
			break;
			case 'g':
				fscanf(f," %f\n",&tmp);
			break;
			*/
			case 'o':
                numberOfObjectsInScene++;
			break;
			case 'v':
				c = fgetc(f);
				if(c=='n')
				{
					fscanf(f," %f %f %f\n",&normals[vnIndex],&normals[vnIndex+1],&normals[vnIndex+2]);
					vnIndex+=3;
				}
				else
				{
					fscanf(f,"%f %f %f\n",&vertices[vIndex],&vertices[vIndex+1],&vertices[vIndex+2]);
					vIndex+=3;
				}
			break;
			case 'f':
				c = fgetc(f);
				b = 2;
				while(c!='\n' && b > 0)
				{
					b=fscanf(f,"%d//%d",&faces[fIndex],&faces[fIndex+1]);
					if(b == 2)
					{
						fIndex += 2;
						c = fgetc(f);
						faceElements[eIndex]++;
					}
				}
				eIndex++;
			break;
			default:
				//c=fgetc(f);
				while(c != '\n' && c != EOF)
					c = fgetc(f);
			break;
		}
		if(c != EOF)
			c = fgetc(f);
	}
	fclose(f);

	printf("inxdices %d %d %d %d %d\n",vIndex,vnIndex,fIndex,eIndex,line);
    printf("objects: %d\n",numberOfObjectsInScene);

	/*
	printf("\nvertices\n");
	for(int i=0;i<vIndex;i++)
	{
		printf(" %f ",vertices[i]);
		vertices[i]=vertices[i]/10.0;
	}
	printf("\nnormals\n");
	for(int i=0;i<vnIndex;i++)
		printf(" %f ",normals[i]);
	printf("\nfaces\n");
	for(int i=0;i<fIndex;i++)
		printf(" %d ",faces[i]);
	*/
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
}

void draw()
{
	for(int i=0, k=0 ; i < fIndex && k < eIndex ; i += faceElements[k]*2, k++)
	{
		glBegin(GL_POLYGON);
        for(int j=0;j<faceElements[k]*2;j+=2)
        {
            glNormal3f(normals[(faces[i+j+1]-1)*3],normals[(faces[i+j+1]-1)*3+1],normals[(faces[i+j+1]-1)*3+2]);
            glVertex3f(vertices[(faces[i+j]-1)*3],vertices[(faces[i+j]-1)*3+1],vertices[(faces[i+j]-1)*3+2]);
        }
		glEnd();
	}
}

void test(void)
{
    glLoadIdentity ();

    glutSwapBuffers();
}

void mydisplay(void)
{
    //  Clear screen and Z-buffer
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    draw();
    draw_axes();

    glutSwapBuffers();
}

int main(int argc, char**argv) 
{
    glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(900, 900);
	glutCreateWindow("Assignment 3");

    reader();
    init();

    // Callback functions
	glutDisplayFunc(mydisplay);
    glutSpecialFunc(specialKeys);
//	glutReshapeFunc(reshape);
//	glutIdleFunc(rotateSun);

    //  Pass control to GLUT for events
    glutMainLoop();

    return 0;
}