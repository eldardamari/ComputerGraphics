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

//#define NORMAL 0
//#define PICK 1
// picking mode
static int pick_x = 0, pick_y = 0;
int pickedItem = -2;
int array[100];
int picked_x[255];
int picked_y[255];
static GLfloat picked_x_mid[255];
static GLfloat picked_y_mid[255];
static GLfloat picked_scale[255];
vector < vector<double> > objCenter;

static int lastx = 0, lasty = 0;
static GLfloat rotate_x = 0;
static GLfloat rotate_y = 0;
static GLfloat  scaling = 1.0f;
static GLfloat global_rotate_x = 0;
static GLfloat global_rotate_y = 0;
static GLfloat global_rotate_x_diff = 0;
static GLfloat global_rotate_y_diff = 0;

static GLfloat cam_rotate_x = 0.01;
static GLfloat cam_rotate_y = 0.01;
static GLfloat cam_rotate_x_temp = 0;
static GLfloat cam_rotate_y_temp = 0;

//middle
static GLfloat mid_global_rotate_x = 0.01;
static GLfloat mid_global_rotate_y = 0;
static GLfloat mid_global_rotate_x_diff = 0;
static GLfloat mid_global_rotate_y_diff = 0;

bool mouse_left   = false;
bool mouse_right  = false;
bool mouse_middle = false;
bool camera_mode    = true;
bool picking_mode   = false;
bool global_mode    = false;
int backspace = 0;
int flag = 0;

int mode = 0;
int color = 255;

#define maxSize 65535

//float rot;
GLfloat vertices[maxSize*3];
GLfloat normals[maxSize*2*3];
GLuint faces[maxSize*4*3];
GLuint faceElements[maxSize*2];

vector< vector<int> > objectColors;

int vIndex,vnIndex,fIndex,eIndex;
FILE *f;
vector<int> objectIndex;

int numberOfObjectsInScene = 0;

GLfloat light_ambient[]     = { 0.5, 0.5, 0.5, 1.0 };
GLfloat light_diffuse[]     = { 0.0, 0.5, 0.5, 1.0 };
GLfloat light_specular[]    = { 0.0, 0.0, 0.5, 1.0 };
GLfloat light_position[]    = { 0, 1.0, 1.0, 0};
GLfloat light_direction[]   = { 0, -1, 0 };
GLfloat shine[]             = { 5.0 };
    
float center_x = 0;
float center_y = 0;
float center_z = 0;

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
    glLightfv(GL_LIGHT0, GL_SHININESS, shine);

    /* switch matrix mode */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();			 //load Identity matrix

    gluPerspective(60,1,0.1,1000);
    gluLookAt(-400,0,0,0,0,0,0,1,0);

	/* return to modelview mode */
	glMatrixMode(GL_MODELVIEW);
}

void addMaterials(  GLfloat a1, GLfloat a2, GLfloat a3,
                    GLfloat d1, GLfloat d2, GLfloat d3,
                    GLfloat s1, GLfloat s2, GLfloat s3  ) 
{

	GLfloat mat_a[] = {a1,a2,a3};
	GLfloat mat_d[] = {d1,d2,d3};
    GLfloat mat_s[] = {s1,s2,s3};

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_a);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,  mat_d);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,  mat_s);
}

void readMaterials(GLfloat materials[]) 
{

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

void reader()
{
	char c;
	f = fopen("./util/scene.obj","r");
	int b = 0;
	vIndex = 0;
	vnIndex = 0;
	fIndex = 0;
	eIndex = 0;
	int line = 0;
	//vector<double> center;
    double x=0, y=0, z=0;
    int firstTime =1;
    int numOfVertices = 0;

    for( int r=0 ; r < 255 ; r++) {
        picked_x[r] = 0;
        picked_y[r] = 0;
        picked_scale[r] = 1;
        picked_x_mid[r] = 0;
        picked_y_mid[r] = 0;
    }



	c = fgetc(f);

	while(  c!=EOF && vIndex  < maxSize*3    &&
	                  vnIndex < maxSize*2*3  &&
	                  fIndex  < maxSize*4*3  )
	{
		line++;
		switch(c)
		{
			case 'o':
                printf("object: %d - vIndex: %d, vnIndex: %d, fIndex: %d, eIndex:%d\n",numberOfObjectsInScene,vIndex,vnIndex,fIndex,eIndex);

                objectIndex.push_back(eIndex);
                numberOfObjectsInScene++;
                if(!firstTime) {
                    vector<double> center;
                    center.push_back(x/(double)numOfVertices);
                    center.push_back(y/(double)numOfVertices);
                    center.push_back(z/(double)numOfVertices);
                    objCenter.push_back(center);

                    x = 0;
                    y = 0;
                    z = 0;
                    numOfVertices = 0;

                }
                firstTime = 0;
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
					x+= vertices[vIndex];
					y+= vertices[vIndex+1];
					z+= vertices[vIndex+2];
				    vIndex+=3;
				    numOfVertices++;
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
    vector<double> center1;

	center1.push_back(x/(double)numOfVertices);
    center1.push_back(y/(double)numOfVertices);
    center1.push_back(z/(double)numOfVertices);
    objCenter.push_back(center1);
	fclose(f);

	//printf("inxdices %d %d %d %d %d\n",vIndex,vnIndex,fIndex,eIndex,line);
    //printf("objects: %d\n",numberOfObjectsInScene);
}

void mouseMotion(int x, int y) 
{
    int delta_x, delta_y;

    // Camera
    if(camera_mode) {
        if(mouse_left) {
            //cout << "in motion -> mouse left " << endl;
            delta_x = (lastx - x);
            delta_y = (lasty - y);

            rotate_x  += (float)(delta_x) / 5.0f;
            rotate_y  -= (float)(delta_y) / 4.0f;

            lastx = x;
            lasty = y;
        }
        if(mouse_right) {
            delta_y = (cam_rotate_y_temp - y);
            cam_rotate_y += (float)delta_y;
            cam_rotate_y_temp = y;
        }

        if(mouse_middle) {
            delta_x = (cam_rotate_x_temp - x);
            cam_rotate_x += (float)delta_x/3.5f;
            cam_rotate_x_temp = x;
        }
    }

    // Global Mode
    if(global_mode) {
            
        if (mouse_left) {
                    global_rotate_y = x - global_rotate_x_diff;
                    global_rotate_x = y + global_rotate_y_diff;
        }

        if (mouse_right) {
                delta_y = lasty - y;
                if ( abs(delta_y) < 10 ){ scaling  += (float)(delta_y) / 100.0f;  }
                if ( scaling < 00.1f) scaling = 0.1f;
                if ( scaling > 10.0f) scaling = 10.0f;
                lasty = y;
            }
        
        if (mouse_middle) {
            mid_global_rotate_x = (mid_global_rotate_x_diff-x);
            mid_global_rotate_y = (mid_global_rotate_y_diff-y);
        }
    }

    // Picking Mode
    if(picking_mode) {

        if(mouse_left) {
          if (pickedItem != -2) {
             picked_x[pickedItem] -= (pick_x-y)/60.0;
             picked_y[pickedItem] -= (pick_y-x)/60.0;
          }
          pick_x = x;
          pick_y = y;
        }

        if (mouse_right) {
            if (pickedItem != -2) {
                picked_scale[pickedItem] += (pick_y - y);
            }
            pick_x = x;
            pick_y = y;
        }

        if (mouse_middle) {
          if (pickedItem != -2) {
             picked_x_mid[pickedItem] += (pick_x-x)/4.0;
             picked_y_mid[pickedItem] += (pick_y-y)/4.0;
          }
          pick_x = x;
          pick_y = y;
        }
    }
    glutPostRedisplay();
}

void mouseClicks(int button, int state, int x,int y) 
{
    switch(button) {
        case GLUT_LEFT_BUTTON:
            mouse_left = true;
            mouse_right = false;
            mouse_middle = false;

            if(state == GLUT_DOWN) {
                if(camera_mode) {
                    //cout << "in camera mode left mouse down " << endl;
                    lastx = x;
                    lasty = y;
                }

                if(global_mode) {
                    global_rotate_x_diff = x - global_rotate_y;
                    global_rotate_y_diff = -y + global_rotate_x;
                }

                if(picking_mode) {
                    pick_x = x;
                    pick_y = y;
                    mode = 1;
                }
            }
            if(state == GLUT_UP) {
                if(picking_mode) {
                    pickedItem = -2;
                }
            }
            break;

        case GLUT_RIGHT_BUTTON:
            mouse_left = false;
            mouse_right = true;
            mouse_middle = false;

            if (state == GLUT_DOWN) {
                if (camera_mode) {
                    cam_rotate_y_temp = y;
                }

                if (global_mode) {
                }

                if (picking_mode) {
                    pick_x = x;
                    pick_y = y;
                    mode = 1;
                }
            }
            if(state == GLUT_UP) {
                if(picking_mode) {
                    pickedItem = -2;
                }
            }
            break;

        case GLUT_MIDDLE_BUTTON:
            mouse_left = false;
            mouse_right = false;
            mouse_middle = true;

            if (state == GLUT_DOWN) {
                if (camera_mode) {
                    cam_rotate_x_temp = x;
                }

                if (global_mode) {
                    mid_global_rotate_x_diff = x;
                    mid_global_rotate_y_diff = y;
                }

                if (picking_mode) {
                    pick_x = x;
                    pick_y = y;
                    mode = 1;
                }
            }
            if(state == GLUT_UP) {
                if(picking_mode) {
                    pickedItem = -2;
                }
            }
            break;
    }
}

void keyboardClicks(unsigned char key, int x, int y) 
{
    switch(key) {
        case 'p':
            cout << "picking mode" << endl;
            camera_mode = false;
            picking_mode = true;
            global_mode = false;
            break;

        case '8':
            picking_mode = false;
            if(backspace) {
                cout << "camera mode" << endl;
                camera_mode = true;
                global_mode = false;
                backspace = 0;
            } else {
                cout << "global mode" << endl;
                global_mode = true;
                camera_mode = false;
                backspace = 1;
            }
            break;

        case 'c':
            picking_mode = false;
            if(backspace) {
                cout << "camera mode" << endl;
                camera_mode = true;
                global_mode = false;
                backspace = 0;
            } else {
                cout << "global mode" << endl;
                global_mode = true;
                camera_mode = false;
                backspace = 1;
            }
            break;

        case 'q':
            exit(1);
            break;
    }
}

void idleCallBack() 
{
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

//    glColor3f (0,0,1); // Z axis is blue.
//    glVertex3fv (ZN);
//    glVertex3fv (ZP);

    glEnd();
}

// Camera and Global transformations for sence and cemera
// picking mode in draw().
void transformations() 
{
        glRotatef(rotate_x, 0.0, 1.0, 0.0);         // camera left click
        glRotatef(rotate_y, 1.0, 0.0, 0.0);         // camera left click
        glTranslatef(cam_rotate_y,0.0,-cam_rotate_x);

        glScalef(scaling, scaling, scaling);        // global right click
        glRotatef(global_rotate_x, 0.0, 1.0, 0.0);  // global left click
        glRotatef(global_rotate_y, 1.0, 0.0, 0.0);  // global left click
        glTranslatef(0.0,0.0,-mid_global_rotate_x);
        //glTranslatef(0.0,mid_global_rotate_y,0.0);
}

void updateObjCenterAfterMotion(int t)
{
    objCenter[t][0] = picked_scale[array[t]]; // updating new center
    /*objCenter[t][1] = picked_x_mid[array[t]]; // updating new center */
    //objCenter[t][2] = picked_x_mid[array[t]]; // updating new center*/
}

void draw()
{
    
    GLfloat materials[numberOfObjectsInScene*9];
    readMaterials(materials);

    glEnable(GL_LIGHTING);

    if (mode == 0) {
        for(int i=0, k=0 ; i < fIndex && k < eIndex ; i += faceElements[k]*2, k++) {

            // check which object is beeing colored
            for(int t=0 ; t < objectIndex.size() ; t++) {

                        center_x = objCenter[t][0];
                        center_y = objCenter[t][1];
                        center_z = objCenter[t][2];
                        
                if(k == objectIndex.at(t)) {
                      if (flag == 1) {
                         glPopMatrix();
                       }
                        glPushMatrix();
                        glLoadIdentity();
                        flag = 1;
                   if(array[t] == pickedItem) {

                   updateObjCenterAfterMotion(t);

                        // Lighter picked Item
                       addMaterials(   materials[t*9]*2.25  , materials[t*9+2]*2.25, materials[t*9+2]*2.25,
                                       materials[t*9+3]*2.25, materials[t*9+2]*2.25, materials[t*9+5]*2.25,
                                       materials[t*9+6]*2.25, materials[t*9+7]*2.25, materials[t*9+8]*2.25 );
                   } else {
                       addMaterials(   materials[t*9]  , materials[t*9+1], materials[t*9+2],
                                       materials[t*9+3], materials[t*9+4], materials[t*9+5],
                                       materials[t*9+6], materials[t*9+7], materials[t*9+8] );
                   }
                   //Transformations
                   updateObjCenterAfterMotion(t);
                   transformations();

                        glTranslatef(center_x,center_y,center_z);
                            glRotatef(picked_x[array[t]], 1.0, 0.0, 0.0);
                            glRotatef(picked_y[array[t]], 0.0, 1.0, 0.0);
                            glTranslatef(picked_scale[array[t]],0.0,0.0);
                            glTranslatef(0.0,0.0,picked_x_mid[array[t]]);
                            /*glTranslatef(0.0,-picked_x_mid[array[t]],-picked_y_mid[array[t]]);*/
                        glTranslatef(-center_x,-center_y,-center_z);
        
                   updateObjCenterAfterMotion(t);
                        
                }
            }
            glBegin(GL_POLYGON);
            for(int j=0 ; j < faceElements[k]*2 ; j+=2) {
                glNormal3f(normals[(faces[i+j+1]-1)*3], normals[(faces[i+j+1]-1)*3+1], normals[(faces[i+j+1]-1)*3+2]);
                glVertex3f(vertices[(faces[i+j]-1)*3], vertices[(faces[i+j]-1)*3+1], vertices[(faces[i+j]-1)*3+2]);
            }
            glEnd();
        }

	} else if(mode == 1) {

        color = 255;
        flag = 0;

     	for(int i=0, k=0 ; i < fIndex && k < eIndex ; i += faceElements[k]*2, k++) {

            for(int t=0 ; t < objectIndex.size() ; t++) {

                if(k == objectIndex.at(t)) {
                      if (flag == 1) {
                          glEnable(GL_LIGHTING);
                         glPopMatrix();
                       }
                        glPushMatrix();
                        glLoadIdentity();
                        glDisable(GL_LIGHTING);
                        flag = 1;
                    array[t]= color-1;
                   
                    //Transformations
                        updateObjCenterAfterMotion(t);
                            transformations();

                        //glTranslatef(center_x,center_y,center_z);
                            glTranslatef(picked_scale[array[t]],0.0,0.0);
                            glTranslatef(0.0,0.0,picked_x_mid[array[t]]);
                        //glTranslatef(-center_x,-center_y,-center_z);
                        updateObjCenterAfterMotion(t);

                    // special color for picking item
                    glColor3ub(color--,0,0);
                }
            }
            glBegin(GL_POLYGON);
            for(int j=0 ; j < faceElements[k]*2 ; j+=2) {
                glVertex3f(vertices[(faces[i+j]-1)*3], vertices[(faces[i+j]-1)*3+1], vertices[(faces[i+j]-1)*3+2]);
            }
            glEnd();
        }
	}
}

void mydisplay(void)
{
    //  Clear screen and Z-buffer
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();			 //load Identity matrix

    draw();

    if(mode == 1) {
        GLint viewport[4];
        GLubyte pixels[3];
        glGetIntegerv(GL_VIEWPORT,viewport);
        glReadPixels(pick_x, viewport[3]-pick_y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE,(void*)pixels);
        pickedItem = pixels[0]-1;
        mode = 0;
    } else {
        glutSwapBuffers();
    }
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
    glutMotionFunc(mouseMotion);
	glutKeyboardFunc(keyboardClicks);
	glutMouseFunc(mouseClicks);
	glutIdleFunc(idleCallBack);

    //  Pass control to GLUT for events
    glutMainLoop();
    return 0;
}

void draw_backup()
{
    GLfloat materials[numberOfObjectsInScene*9];
    readMaterials(materials);
    float center_x = 0;
    float center_y = 0;
    float center_z = 0;

    glEnable(GL_LIGHTING);

    if (mode == 0) {
        for(int i=0, k=0 ; i < fIndex && k < eIndex ; i += faceElements[k]*2, k++) {

            // check which object is beeing colored
            for(int t=0 ; t < objectIndex.size() ; t++) {

                        center_x = objCenter[t][0];
                        center_y = objCenter[t][1];
                        center_z = objCenter[t][2];
                        
                if(k == objectIndex.at(t)) {
                      if (flag == 1) {
                         glPopMatrix();
                       }
                        glPushMatrix();
                        glLoadIdentity();
                        flag = 1;
                   if(array[t] == pickedItem) {
                      /*if (flag == 1) {
                          cout << "handling object number: " << pickedItem << " k== " << k << endl;
                         glPopMatrix();
                        //flag =0;
                       }
                        glPushMatrix();
                        glLoadIdentity();
                        flag = 1;*/


                        /*center_x = objCenter[t][0];
                        center_y = objCenter[t][1];
                        center_z = objCenter[t][2];

                        glTranslatef(center_x,center_y,center_z);
                            glRotatef(picked_x[pickedItem], 1.0, 0.0, 0.0);
                            glRotatef(picked_y[pickedItem], 0.0, 1.0, 0.0);
                        glTranslatef(-center_x,-center_y,-center_z);*/

                        /*glTranslatef(center_x,center_y,center_z);
                            glRotatef(picked_x[array[t]], 1.0, 0.0, 0.0);
                            glRotatef(picked_y[array[t]], 0.0, 1.0, 0.0);
                        glTranslatef(-center_x,-center_y,-center_z);
                        glTranslatef(picked_scale[array[t]],0.0,0.0);*/
                        objCenter[t][0] = picked_scale[array[t]]; // updating new center


                       /*cout << "match:  " << pickedItem << endl;*/
                       addMaterials(   materials[t*9]*2.25  , materials[t*9+2]*2.25, materials[t*9+2]*2.25,
                                       materials[t*9+3]*2.25, materials[t*9+2]*2.25, materials[t*9+5]*2.25,
                                       materials[t*9+6]*2.25, materials[t*9+7]*2.25, materials[t*9+8]*2.25 );
                   } else {
                       //glPopMatrix();
                        /*glTranslatef(center_x,center_y,center_z);
                            glRotatef(picked_x[array[t]], 1.0, 0.0, 0.0);
                            glRotatef(picked_y[array[t]], 0.0, 1.0, 0.0);
                        glTranslatef(-center_x,-center_y,-center_z);
                            glScalef(picked_scale[array[t]], picked_scale[array[t]], picked_scale[array[t]]);*/

                       addMaterials(   materials[t*9]  , materials[t*9+1], materials[t*9+2],
                                       materials[t*9+3], materials[t*9+4], materials[t*9+5],
                                       materials[t*9+6], materials[t*9+7], materials[t*9+8] );
                   }
                        glTranslatef(center_x,center_y,center_z);
                            glRotatef(picked_x[array[t]], 1.0, 0.0, 0.0);
                            glRotatef(picked_y[array[t]], 0.0, 1.0, 0.0);
                        glTranslatef(-center_x,-center_y,-center_z);
                        glTranslatef(picked_scale[array[t]],0.0,0.0);
                        objCenter[t][0] = picked_scale[array[t]]; // updating new center
                }
            }
            glBegin(GL_POLYGON);
            for(int j=0 ; j < faceElements[k]*2 ; j+=2) {
                glNormal3f(normals[(faces[i+j+1]-1)*3], normals[(faces[i+j+1]-1)*3+1], normals[(faces[i+j+1]-1)*3+2]);
                glVertex3f(vertices[(faces[i+j]-1)*3], vertices[(faces[i+j]-1)*3+1], vertices[(faces[i+j]-1)*3+2]);
            }
            glEnd();
        }

	} else if(mode == 1) {

        glDisable(GL_LIGHTING);
        color = 255;
        flag = 0;

     	for(int i=0, k=0 ; i < fIndex && k < eIndex ; i += faceElements[k]*2, k++) {

            for(int t=0 ; t < objectIndex.size() ; t++) {

                if(k == objectIndex.at(t)) {
                    if (flag == 1) {
                        glPopMatrix();
                        flag =0;
                    }
                    glPushMatrix();
                    glDisable(GL_LIGHTING);
                    flag = 1;
                    array[t]= color-1;
                    glColor3ub(color--,0,0);
                }
            }
            glBegin(GL_POLYGON);
            for(int j=0 ; j < faceElements[k]*2 ; j+=2) {
                glVertex3f(vertices[(faces[i+j]-1)*3], vertices[(faces[i+j]-1)*3+1], vertices[(faces[i+j]-1)*3+2]);
            }
            glEnd();
        }
        glEnable(GL_LIGHTING);
//         flag = 0;
//        for(int i=0, k=0 ; i < fIndex && k < eIndex ; i += faceElements[k]*2, k++) {
//            // check which object is beeing colored
//            for(int t=0 ; t < objectIndex.size() ; t++) {
//                if(k == objectIndex.at(t)) {
//                    if (flag = 1) {
//                        glPopMatrix();
//                        flag = 0;
//                    }
//                    glPushMatrix();
//                    flag = 1;
//                    if(array[t] == pickedItem)
//                    {
//                        cout << "match  " << pickedItem << endl;
//                        addMaterials(   materials[t*9]*1.15  , materials[t*9+1]*1.15, materials[t*9+2]*1.15,
//                                        materials[t*9+3]*1.15, materials[t*9+4]*1.15, materials[t*9+5]*1.15,
//                                        materials[t*9+6]*1.15, materials[t*9+7]*1.15, materials[t*9+8]*1.15 );
//                    } else {
//                        addMaterials(   materials[t*9]  , materials[t*9+1], materials[t*9+2],
//                                        materials[t*9+3], materials[t*9+4], materials[t*9+5],
//                                        materials[t*9+6], materials[t*9+7], materials[t*9+8] );
//                    }
//                }
//            }
//
//            glBegin(GL_POLYGON);
//            for(int j=0 ; j < faceElements[k]*2 ; j+=2) {
//                glNormal3f(normals[(faces[i+j+1]-1)*3], normals[(faces[i+j+1]-1)*3+1], normals[(faces[i+j+1]-1)*3+2]);
//                glVertex3f(vertices[(faces[i+j]-1)*3], vertices[(faces[i+j]-1)*3+1], vertices[(faces[i+j]-1)*3+2]);
//            }
//            glEnd();
//
//            //glPopMatrix();
//        }

	}
}

