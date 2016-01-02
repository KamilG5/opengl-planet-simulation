#include <stdio.h>     // - Just for some ASCII messages
#include <string.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include "visuals.h"   // Header file for our OpenGL functions
#include <stdlib.h>
#ifdef __linux__ 
#include <GL/glut.h>
#elif _WIN32
#include "gl/glut.h"   // - An interface and windows management library
#endif
model md;
static float tx = 0.0;
static float rotx = 0.0;
static bool animate = false;
static float red = 1.0;
static float green = 0.0;
static float blue = 0.0;


static bool up = true;
static bool star_up = true;
#define RADIUS_INIT 1.4
#define RADIUS_THRESHOLD 0.2
#define RADIUS_STEP 0.01

static double sun_radius = RADIUS_INIT;

#define STARS_NUM 50
#define STARS_INIT 1.5
#define STARS_THRESHOLD 2
#define STARS_STEP 0.005

static double star_radius = STARS_INIT;

double stars_positions[STARS_NUM][2];
double stars_radius[STARS_NUM];
double stars_radius_init[STARS_NUM];

using namespace std;

void keimeno(const char *str,float size)
{

	glPushMatrix();
	glScalef(size,size,size);

	for (int i=0;i<strlen(str);i++)
	  glutStrokeCharacter(GLUT_STROKE_ROMAN,str[i]);
	glPopMatrix();

}

void Render()
{    
  //CLEARS FRAME BUFFER ie COLOR BUFFER& DEPTH BUFFER (1.0)
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // Clean up the colour of the window
													   // and the depth buffer
  glMatrixMode(GL_MODELVIEW); 
  glLoadIdentity();

  //glTranslatef(0,0,-100);
  //glTranslatef(tx,0.0,0.0);
  //glRotatef(rotx,1,0,0);

  
 
  //(01)             
// DisplayModel(md);
	DisplayStars();
	DisplaySun();

  //(02)
  //glColor3f(0.8, 0.1, 0.1);
  //glTranslatef(-20.0,0.0,0.0);
  //keimeno("Dokimastiko keimeno",0.05f);

  //(03)             
  //glColor3f(red, green, blue);                            // Set drawing colour
  //glutSolidTeapot(20.0);
  
 
  glutSwapBuffers();             // All drawing commands applied to the 
                                 // hidden buffer, so now, bring forward
                                 // the hidden buffer and hide the visible one
}

//-----------------------------------------------------------

void Resize(int w, int h)
{ 
  // define the visible area of the window ( in pixels )
  if (h==0) h=1;
  glViewport(0,0,w,h); 

  // Setup viewing volume

  glMatrixMode(GL_PROJECTION); 
  glLoadIdentity();
 
  gluPerspective(60.0, (float)w/(float)h, 1.0, 500.0);
}

void Idle()
{

	for (int i = 0; i < STARS_NUM ; i++) {

		if ((stars_radius[i] < stars_radius_init[i] + STARS_THRESHOLD * stars_radius_init[i]) && star_up) {
			stars_radius[i] += STARS_STEP;
		}
		else if ((stars_radius[i] > stars_radius_init[i]) && !star_up) {
			stars_radius[i] -= STARS_STEP;

		}
		else if (stars_radius[i] >= stars_radius_init[i] + STARS_THRESHOLD*stars_radius_init[i] && star_up) {
			star_up = false;
			stars_radius[i] -= STARS_STEP;

		}
		else if (stars_radius[i] <= stars_radius_init[i] && !star_up) {
			star_up = true;
			stars_radius[i] += STARS_STEP;
		}

	}

	if ( (sun_radius < RADIUS_INIT + RADIUS_THRESHOLD) && up) {
		sun_radius += RADIUS_STEP;
	}
	else if( (sun_radius > RADIUS_INIT ) && !up ) {
		sun_radius -= RADIUS_STEP;
		
	}else if (sun_radius >= RADIUS_INIT + RADIUS_THRESHOLD && up){
		up = false;
		sun_radius -= RADIUS_STEP;
		
	}
	else if (sun_radius <= RADIUS_INIT && !up) {
		up = true;
		sun_radius += RADIUS_STEP;
	}
	glutPostRedisplay();
}

void Keyboard(unsigned char key,int x,int y)
{
	switch(key)
	{
	case 'q' : exit(0);
		break;
	case 'a' : tx-=0.5f;
		break;
	case 'd' : tx+=0.5f;
		break;
	default : break;
	}

	glutPostRedisplay();

}

void Mouse(int button,int state,int x,int y)
{
	 if(state == GLUT_DOWN && button == GLUT_LEFT_BUTTON)
	 {
		 animate = !animate;
         glutPostRedisplay();
	 }
}


void Setup()  // TOUCH IT !! 
{ 
	for (int i = 0; i < STARS_NUM; i++) {
		stars_radius[i] = stars_radius_init[i] = rand() / (((double)RAND_MAX*20.0)) + 0.01;

		stars_positions[i][0] = (rand() % 10000) / 1000.0;
		stars_positions[i][1] = (rand() % 10000) /  1000.0;



		if (i % 4 == 0) {
			stars_positions[i][0] *= -1;
			stars_positions[i][1] *= -1;
		}
		else if (i % 4 == 1) {
			stars_positions[i][0] *= -1;
		}
		else if (i % 4 == 2) {
			stars_positions[i][1] *= -1;
		}


		//cout <<i <<"radius: "<< stars_radius[i] <<" x: " << stars_positions[i][0] << " y: " << stars_positions[i][1] << endl;

	}
	ReadFile(&md);
	glShadeModel(GL_FLAT);


	// polygon rendering mode
	//glEnable(GL_COLOR_MATERIAL);
	//glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	//
	////Set up light source
	//GLfloat light_position[] = { 0, 0.0, 0.0, 1.0 };
	//GLfloat ambientLight[] = { 0.2, 0.2, 0.2, 1.0 };
	//GLfloat diffuseLight[] = { 0.8, 0.8, 0.8, 1.0 };
	//
	//glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
	//glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	//glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	//
	//glEnable(GL_LIGHTING);
	//glEnable(GL_LIGHT0);
	//
	//glEnable(GL_POINT_SMOOTH);

	// Black background
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);








	//Parameter handling
	glShadeModel (GL_SMOOTH);
	
	//glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);  //renders a fragment if its z value is less or equal of the stored value
	//glClearDepth(1);
    
	// polygon rendering mode
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColorMaterial( GL_FRONT, GL_AMBIENT_AND_DIFFUSE );
  
	//Set up light source
	GLfloat light_position[] = { 0.0, 0.0, -5.0, 0.0 };
	glLightfv( GL_LIGHT0, GL_POSITION, light_position);

	GLfloat ambientLight[] = { 0.3, 0.3, 0.3, 1.0 };
	GLfloat diffuseLight[] = { 0.8, 0.8, 0.8, 1.0 };
	GLfloat specularLight[] = { 1.0, 1.0, 1.0, 1.0 };

	   
	glLightfv( GL_LIGHT0, GL_AMBIENT, ambientLight );
	glLightfv( GL_LIGHT0, GL_DIFFUSE, diffuseLight );
	

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	//GLfloat light_position1[] = { 0.0, 0.0, 1.0, 0.0 };
	//glLightfv(GL_LIGHT1, GL_POSITION, light_position1);
	//
	//GLfloat ambientLight1[] = { 0.3, 0.3, 0.3, 1.0 };
	//GLfloat diffuseLight1[] = { 0.8, 0.8, 0.8, 1.0 };
	//GLfloat specularLight1[] = { 1.0, 1.0, 1.0, 1.0 };
	//
	//
	//glLightfv(GL_LIGHT1, GL_AMBIENT, ambientLight1);
	//glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuseLight1);
	//
	//
	//glEnable(GL_LIGHTING);
	//glEnable(GL_LIGHT1);
	//
	//glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	
	//glEnable(GL_CULL_FACE);
	//glFrontFace(GL_CW);

	//01
	//glFrontFace(GL_CCW);
 

	// Black background
	//glClearColor(0.0f,0.0f,0.0f,1.0f);

}

void DisplaySun() {
	glPushMatrix();
		glTranslatef(0, 0, -10);
		double opacity = (RADIUS_INIT + RADIUS_THRESHOLD - sun_radius) / RADIUS_THRESHOLD + 0.35;
		glColor4f(0.3, 0.33, 0.0, opacity);							   // Radiation
		glutSolidSphere(sun_radius, 200, 20);

		glColor3f(1, 0.91, 0.0);							   // Sun
		glutSolidSphere(RADIUS_INIT, 200, 20);
		
	
		
		
		glPopMatrix();
	

	
}

void DisplayStars() {
	//double star_radius = (rand() / (((double)RAND_MAX)*10.0));
	//double star_radius =0.5;
	for (int i = 0; i < STARS_NUM; i++) {
		glPushMatrix();

		glTranslatef(stars_positions[i][0], stars_positions[i][1], -20);
		double opacity = (stars_radius_init[i] + STARS_THRESHOLD - stars_radius[i]) / STARS_THRESHOLD;

		//cout << i << ": opacity (" <<opacity << ") star_radius (" <<stars_radius[i] <<")"<<"star_radius_init("<<stars_radius_init[i]<<")"<< endl;
		glColor4f(0.5, 0.5, 0.5, opacity);							   // Set drawing colour = orange
		glutSolidSphere(stars_radius[i], 200, 20);

		glColor3f(1, 1, 1);							   // Set drawing colour = orange
		glutSolidSphere(stars_radius_init[i], 200, 20);

		glPopMatrix();

	}

	

	
}

void MenuSelect(int choice)
{
	switch (choice) {
		case RED : 
			red = 1.0; 
			green = 0.0; 
			blue = 0.0; break;
		case GREEN : 
			red = 0.0; 
			green = 1.0; 
			blue = 0.0; break;
		case BLUE : 
			red = 0.0; 
			green = 0.0; 
			blue = 1.0; break;
		case WHITE : 
			red = 1.0; 
			green = 1.0; 
			blue = 1.0; break;
	}

}

void ReadFile(model *md)
{


	FILE * file = fopen("planet.obj", "r");
	if (file == NULL) {
		printf("Impossible to open the file !\n");
		return;
	}
   
	while (1) {

		char lineHeader[1024];
		// read the first word of the line
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
			break; // EOF = End Of File. Quit the loop.
	   // else : parse lineHeader

		if (strcmp(lineHeader, "v") == 0) {
			point vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			md->obj_points.push_back(vertex);
		}
		else if (strcmp(lineHeader, "vn") == 0) {
			point normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			md->normals.push_back(normal);
		}
		else if (strcmp(lineHeader, "f") == 0) {
			face vertexIndex, normalIndex;
			int matches = fscanf(file, "%d//%d %d//%d %d//%d\n", &vertexIndex.vtx[0], &normalIndex.vtx[0], &vertexIndex.vtx[1], &normalIndex.vtx[1], &vertexIndex.vtx[2], &normalIndex.vtx[2]);
			if (matches != 6) {
				cout << "Oops" << endl;
				return;
			}

			md->vertexIndices.push_back(vertexIndex);

			md->normalIndices.push_back(normalIndex);
		}
	}

	 fclose(file);
}

void DisplayModel(model md){
	glColor3f(0.3, 0.2, 0.9);                            // Set drawing colour

	glTranslatef(0,0,-200);
	glPushMatrix();
	glBegin(GL_TRIANGLES);

	for (int i = 0; i < md.vertexIndices.size(); i++)
	{
		glVertex3f(md.obj_points[md.vertexIndices[i].vtx[0]-1].x,md.obj_points[md.vertexIndices[i].vtx[0]-1].y,md.obj_points[md.vertexIndices[i].vtx[0]-1].z);
		glVertex3f(md.obj_points[md.vertexIndices[i].vtx[1]-1].x,md.obj_points[md.vertexIndices[i].vtx[1]-1].y,md.obj_points[md.vertexIndices[i].vtx[1]-1].z);
		glVertex3f(md.obj_points[md.vertexIndices[i].vtx[2]-1].x,md.obj_points[md.vertexIndices[i].vtx[2]-1].y,md.obj_points[md.vertexIndices[i].vtx[2]-1].z);
	}
	
	glEnd();
	glPopMatrix();
	
}