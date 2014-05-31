#include <stdio.h>
#include "glut.h"
#define maxSize 65535 
float rot;
GLfloat vertices[maxSize*3];
GLfloat normals[maxSize*2*3];
GLuint faces[maxSize*4*3];
GLuint faceElements[maxSize*2];
int vIndex,vnIndex,fIndex,eIndex;
FILE *f;

void reader()
{
	char c;
	f=fopen("doll.obj","r");
	int b=0;
	float tmp;
	vIndex=0;
	vnIndex=0;
	fIndex=0;
	eIndex=0;
	int line=0;
	c=fgetc(f);
	while(c!=EOF && vIndex<maxSize*3 && vnIndex<maxSize*2*3 && fIndex<maxSize*4*3)
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
			case 'v':
				c=fgetc(f);
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
				c=fgetc(f);
				b=2;
				while(c!='\n' && b>0)
				{
					b=fscanf(f,"%d//%d",&faces[fIndex],&faces[fIndex+1]);	
					if(b==2)
					{
						fIndex+=2;
						c=fgetc(f);
						faceElements[eIndex]++;
					}
				}
				eIndex++;
			break;
			default:
				//c=fgetc(f);
				while(c!='\n' && c!=EOF)
					c=fgetc(f);
			break;
		}
		if(c!=EOF)
			c=fgetc(f);
	}
	fclose(f);
	printf("indices %d %d %d %d %d\n",vIndex,vnIndex,fIndex,eIndex,line);
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

void draw()
{
	for(int i=0,k=0;i<fIndex && k<eIndex;i+=faceElements[k]*2,k++)
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

void init()
{

	glClearColor(0,0,0,1);  //black
	
	//enable tests
	//glShadeModel(GL_FLAT);

	glEnable(GL_DEPTH_TEST);
    glEnable (GL_LIGHTING);
    glEnable (GL_LIGHT0);
	//GLfloat light_direction[]={0,-1,0};
	GLfloat light_ambient[] = {0.5, 0.5, 0.5, 1.0}; //color
	GLfloat light_diffuse[] = {0.0, 0.5, 0.5, 1.0}; //color
	GLfloat light_specular[] = {0.0, 0.0, 0.5, 1.0}; 
	GLfloat light_position[]={0,1.0,1,0};
	//GLfloat angle[] = {20.0};
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	rot=0;

	GLfloat mat_a[] = {0.3, 0.4, 0.5, 1.0};
	GLfloat mat_d[] = {0.0, 0.6, 0.7, 1.0};
	GLfloat mat_s[] = {0.0, 0.0, 0.8, 1.0};
	GLfloat low_sh[] = {5.0};
	
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_a);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_d);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_s);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, low_sh);
	
	reader();
}


void display(void)
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
	glLoadIdentity();
	glRotatef(rot,0,1,0);
    glutSolidSphere(1.5,32,32);
    //glutSwapBuffers();
	draw();
    rot += .01f;
	glFlush();
}

void reshape( int width, int height )
{
    glViewport(0, 0, (GLsizei)width, (GLsizei)height );
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, (GLfloat)width / (GLfloat)height, 1.0, 200.0);
	glTranslatef(0.0f, -50, -100.0f);
	//glRotatef(180,1,0,0);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(150, 150);
    glutCreateWindow("obj");

    init();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(display);
    glutMainLoop();

    return 0;
}
