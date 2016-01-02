#define RED 1
#define GREEN 2
#define BLUE 3
#define WHITE 4
#include <vector>



struct point 
{
	float x; 
	float y; 
	float z; 
};

struct face
{
	int vtx[3];
};

struct model
{

	std::vector< face > vertexIndices, normalIndices;
	std::vector< point > obj_points;
	std::vector< point > normals;

	//	point obj_points[9122];
	//	face obj_faces[18240];
	//	int vertices;
	//	int faces;
};



//-------- Functions --------------------------------

void Render();
// The function responsible for drawing everything in the 
// OpenGL context associated to a window. 

void Resize(int w, int h);
// Handle the window size changes and define the world coordinate 
// system and projection type

void Setup();
// Set up the OpenGL state machine and create a light source

void Idle();

void ReadFile(model*);
//Function for reading a model file

void DisplayModel(model);
// Function for displaying a model

void Keyboard(unsigned char key,int x,int y);
// Function for handling keyboard events.

void Mouse(int button,int state,int x,int y); 
// Function for handling mouse events

void MenuSelect(int choice);
void DisplaySun();
void DisplayStars();
void displayPlanet(model, double, double, double, float);
