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
#define PLANETS 4

static float tx = 0.0;
static float ty = 0.0;
static float tz = 0.0;
static float planet_rotate = 0.0;
static float rotx[PLANETS];
static bool animate = true;
static float red = 1.0;
static float green = 0.0;
static float blue = 0.0;


static bool up = true;
static bool star_up = true;
#define RADIUS_INIT 1.6
#define RADIUS_THRESHOLD 0.24
#define RADIUS_STEP 0.01

static double sun_radius = RADIUS_INIT;


#define STARS_NUM 100
#define STARS_INIT 1.5
#define STARS_THRESHOLD 0.02
#define STARS_STEP 0.003



static double star_radius = STARS_INIT;

double stars_positions[STARS_NUM][3];
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

  glTranslatef(0, 0, -15);
  glRotatef(tx, 1, 0, 0);
  glRotatef(ty, 0, 1, 0);
  glTranslatef(0, 0, 15);

  
 
	DisplayStars();
	DisplayModel(md);
	DisplaySun();
  
 
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
	if(animate){
		for (int i = 0; i < STARS_NUM ; i++) {

			if ((stars_radius[i] < stars_radius_init[i] + STARS_THRESHOLD) && star_up) {
				stars_radius[i] += STARS_STEP;
			}
			else if ((stars_radius[i] > stars_radius_init[i]) && !star_up) {
				stars_radius[i] -= STARS_STEP;

			}
			else if (stars_radius[i] >= stars_radius_init[i] + STARS_THRESHOLD && star_up) {
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

		planet_rotate += 6.0;
		for(int i = 0; i < PLANETS; i++){
			rotx[i] += 1.2+ 2.0*i;
		}
	}

	glutPostRedisplay();
}



void Keyboard(unsigned char key,int x,int y)
{
	switch(key)
	{
	case 'q' : exit(0);
		break;
	case 'a' : ty-=0.5f;
			   // tz+=0.5f;
		break;
	case 'd' : ty+=0.5f;
			   // tz-=0.5f;
		break;
	case 'w' : tx-=0.5f;
		break;
	case 's' : tx+=0.5f;
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
	for(int i = 0; i < PLANETS; i++){
		rotx[i] = 0.0;
	}


	for (int i = 0; i < STARS_NUM; i++) {
		stars_radius[i] = stars_radius_init[i] = rand() / (((double)RAND_MAX*30.0)) + 0.01;

		stars_positions[i][0] = (rand() % 10000) / 600.0;
		stars_positions[i][1] = (rand() % 10000) / 600.0;
		stars_positions[i][2] = (rand() % 50);

		if(rand()%2==0) 
			stars_positions[i][2] *= -1;


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



	}
	ReadFile(&md);
	glShadeModel(GL_FLAT);


	// polygon rendering mode
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);



	//No idea why
	glEnable(GL_NORMALIZE);




	//Parameter handling
	glShadeModel (GL_SMOOTH);
	
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);  //renders a fragment if its z value is less or equal of the stored value
	glClearDepth(1);
    
	// polygon rendering mode
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColorMaterial( GL_FRONT, GL_AMBIENT_AND_DIFFUSE );
  
	//Set up light source
	GLfloat light_position[] = { 0.0, 0.0, 15.0, 0.0 };
	glLightfv( GL_LIGHT0, GL_POSITION, light_position);

	GLfloat ambientLight[] = { 0.3, 0.3, 0.3, 1.0 };
	GLfloat diffuseLight[] = { 0.8, 0.8, 0.8, 1.0 };
	GLfloat specularLight[] = { 1.0, 1.0, 1.0, 1.0 };

	   
	glLightfv( GL_LIGHT0, GL_AMBIENT, ambientLight );
	glLightfv( GL_LIGHT0, GL_DIFFUSE, diffuseLight );
	

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glFrontFace(GL_CW);


}

void DisplaySun() {
	glPushMatrix();
		glTranslatef(0, 0, -15);

		glColor3f(1, 0.91, 0.0);							   // Sun
		glutSolidSphere(RADIUS_INIT, 200, 20);

		double opacity = (RADIUS_INIT + RADIUS_THRESHOLD - sun_radius) / RADIUS_THRESHOLD + 0.35;
		glColor4f(1, 0.91, 0.0, opacity);							   // Radiation
		glutSolidSphere(sun_radius, 200, 20);
	glPopMatrix();
	
}

void DisplayStars() {
	for (int i = 0; i < STARS_NUM; i++) {
		glPushMatrix();
			glTranslatef(stars_positions[i][0], stars_positions[i][1], stars_positions[i][2]);

			glColor3f(1, 1, 1);							   
			glutSolidSphere(stars_radius_init[i], 200, 20);

			double opacity = (stars_radius_init[i] + STARS_THRESHOLD - stars_radius[i]) / STARS_THRESHOLD;
			// printf("op: %lf, stars_radius_init[i]:%lf, stars_radius[i]: %lf, STARS_THRESHOLD: %lf   \n",opacity, stars_radius_init[i], stars_radius[i], STARS_THRESHOLD );
			glColor4f(0.5, 0.5, 0.5, opacity);							   
			glutSolidSphere(stars_radius[i], 200, 20);

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

	glColor3f(0.046, 0.897, 0.247);                            // Set drawing colour
	displayPlanet(md, 0, 0, 0, 0.0015, 0);

	glColor3f(0.945, 0.081, 0.12);                            // Set drawing colour
	displayPlanet(md, 0, 0, 0, 0.0020, 1);

	glColor3f(0.53, 0.27, 0.94);                            // Set drawing colour
	displayPlanet(md, 0, 0, 0, 0.0018, 2);

	glColor3f(0.3, 0.2, 0.9);                            // Set drawing colour
	displayPlanet(md, 0, 0, 0, 0.0014, 3);


	
	
}

void displayPlanet(model md, double x, double y, double z, float size, int planet_no){
	glPushMatrix();
	glTranslatef(x,y,z);
		if(planet_no % 4 == 0){
			glTranslatef(0, 0, -15); 
			glRotatef(rotx[planet_no], 0, 1, 0);
			glTranslatef(0, 0, 9);
		} else if(planet_no % 4 == 3) {
			glTranslatef(0, 0, -20); 
			glRotatef(rotx[planet_no], 1, 1, 0);
			glTranslatef(0, 0, 14);
		} else if(planet_no % 4 == 1){
			glTranslatef(0, 0, -19); 
			glRotatef(rotx[planet_no], 1, 0, 0);
			glTranslatef(0, 0, 13);
		} else if(planet_no % 4 == 2){
			glTranslatef(0, 0, -15); 
			glRotatef(rotx[planet_no], -1, 1, 0);
			glTranslatef(0, 0, 9);
		}

	glScalef(size, size, size);
	glRotatef(planet_rotate, 1, 0, 0);
	glTranslatef(-x,-y,-z);
	glBegin(GL_TRIANGLES);

	for (int i = 0; i < md.vertexIndices.size(); i++)
	{
		glVertex3f(md.obj_points[md.vertexIndices[i].vtx[0]-1].x,md.obj_points[md.vertexIndices[i].vtx[0]-1].y,md.obj_points[md.vertexIndices[i].vtx[0]-1].z);
		glVertex3f(md.obj_points[md.vertexIndices[i].vtx[1]-1].x,md.obj_points[md.vertexIndices[i].vtx[1]-1].y,md.obj_points[md.vertexIndices[i].vtx[1]-1].z);
		glVertex3f(md.obj_points[md.vertexIndices[i].vtx[2]-1].x,md.obj_points[md.vertexIndices[i].vtx[2]-1].y,md.obj_points[md.vertexIndices[i].vtx[2]-1].z);

		glNormal3f(md.obj_points[md.normalIndices[i].vtx[0]-1].x,md.obj_points[md.normalIndices[i].vtx[0]-1].y,md.obj_points[md.normalIndices[i].vtx[0]-1].z);
		glNormal3f(md.obj_points[md.normalIndices[i].vtx[1]-1].x,md.obj_points[md.normalIndices[i].vtx[1]-1].y,md.obj_points[md.normalIndices[i].vtx[1]-1].z);
		glNormal3f(md.obj_points[md.normalIndices[i].vtx[2]-1].x,md.obj_points[md.normalIndices[i].vtx[2]-1].y,md.obj_points[md.normalIndices[i].vtx[2]-1].z);
	}
	
	glEnd();
	glPopMatrix();
}