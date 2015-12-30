//--------------------------------------------------//
//													//	
//    Learning OpenGL: Εισαγωγικά Μαθήματα OpenGL   //
//													//
//    Πλατφόρμα ανάπτυξης παραδειγμάτων				//
//													//
//													//
//		Μάθημα 5ο									//
//--------------------------------------------------//


#include <stdio.h>     // - Just for some ASCII messages
#include "visuals.h"   // Header file for our OpenGL functions
#ifdef __linux__ 
#include <GL/glut.h>
#elif _WIN32
#include "gl/glut.h"   // - An interface and windows 
                         management library
#endif
#include <time.h>


////////////////// State Variables ////////////////////////


/////////////// Main Program ///////////////////////////

int main(int argc, char* argv[])
{ 
	srand(time(NULL));
  // initialize GLUT library state
  glutInit(&argc, argv);
	
  // Set up the display using the GLUT functions to 
  // get rid of the window setup details:
  // - Use true RGB colour mode ( and transparency )
  // - Enable double buffering for faster window update
  // - Allocate a Depth-Buffer in the system memory or 
  //   in the video memory if 3D acceleration available	
                      //RGBA//DEPTH BUFFER//DOUBLE BUFFER//
  glutInitDisplayMode(GLUT_RGBA|GLUT_DEPTH|GLUT_DOUBLE);
 
  
  // Define the main window size and initial position 
  // ( upper left corner, boundaries included )
  glutInitWindowSize(1200,1200);
  glutInitWindowPosition(50,50);
  
  // Create and label the main window
  glutCreateWindow("Planet Simulation");
  
  // Configure various properties of the OpenGL rendering context
  Setup();
  
  // Callbacks for the GL and GLUT events:

  // The rendering function 
  glutDisplayFunc(Render);
  glutReshapeFunc(Resize);
  glutIdleFunc(Idle);
  //glutKeyboardFunc(Keyboard);
  //glutMouseFunc(Mouse);

//  glutCreateMenu(MenuSelect);
 // glutAddMenuEntry("Red",RED);
  //glutAddMenuEntry("Blue",BLUE);
  //glutAddMenuEntry("Green",GREEN);
  //glutAddMenuEntry("White",WHITE);
	
  // attach the menu to the right button
//  glutAttachMenu(GLUT_RIGHT_BUTTON);

 
  //Enter main event handling loop
  glutMainLoop();
  return 0;	
}  
