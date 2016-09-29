//
//  main.cpp
//  GlutTest
//
//  Created by Gaurav Mishra on 28/09/16.
//  Copyright © 2016 Gaurav Mishra. All rights reserved.
//

#include <iostream>
#include <GLUT/GLUT.h>
#include "DrawableObject.hpp"
#include "Line.hpp"
#include <vector>
using namespace std;



const int WINDOW_HEIGHT = 480;
const int WINDOW_WIDTH = 640;

const int UI_MODE_LINE = 0;
const int UI_MODE_FREEHAND = 1;

vector< DrawableObject* > drawableObjects;

Line *currentLine;

// 0 -- line Mode , 1 -- free hand
int UIMode = 0;

struct Mouse
{
    int x;		/*	the x coordinate of the mouse cursor	*/
    int y;		/*	the y coordinate of the mouse cursor	*/
    int mousePressed ;	/*	is the left button pressed?		*/
    
    
    int xDragStart; /*	stores the x-coord of when the first button press occurred	*/
    int yDragStart; /*	stores the y-coord of when the first button press occurred	*/
};

struct Mouse mouseController{0,0,0,0,0};


/* initialize opengl */
void init()
{
    glEnable(GL_LIGHT0);
    glClearColor (1.0, 1.0, 1.0, 0.0); // Set display-window color to white.
    glMatrixMode (GL_PROJECTION); // Set projection parameters.
//    gluOrtho2D (0.0, 800, 0.0, 600);
    
}

/*----------------------------------------------------------------------------------------
 *	draw 3d objects in the scene */
void draw3D()
{
    gluLookAt(0,1,5,0,0,0,0,1,0);
    glutSolidTeapot(1);
}

/*----------------------------------------------------------------------------------------
 *	draw 2d objects in the scene.
 */
void draw2D()
{
    
    for (DrawableObject* obj: drawableObjects){
        obj->draw();
    }
    
    if(currentLine != NULL){
        currentLine->draw();
    }
    
    glFlush ( ); // Process all OpenGL routines as quickly as possible.

}



void draw (void)
{
    /*
     *	Clear the background
     */
    glClear( GL_COLOR_BUFFER_BIT |
            GL_DEPTH_BUFFER_BIT );
    
    /*
     *	Enable lighting and the z-buffer
     */
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    
    /*
     *	Set perspective viewing transformation
     */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45,(WINDOW_HEIGHT==0)?(1):((float)WINDOW_WIDTH/WINDOW_HEIGHT),1,100);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    /*
     *	Draw the 3D elements in the scene
     */
    //draw3D();
    
    /*
     *	Disable depth test and lighting for 2D elements
     */
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    
    /*
     *	Set the orthographic viewing transformation
     */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0,WINDOW_WIDTH,WINDOW_HEIGHT,0,-1,1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    /*
     *	Draw the 2D overlay
     */
    draw2D();
    
    /*
     *	Bring the back buffer to the front and vice-versa.
     */
    glutSwapBuffers();

}

/*******************************************************************************************************************************
                                    Current Line Controling Methods
 *******************************************************************************************************************************/

/*
 will be called when user moves the mouse while mouse is down.
 */
void updateCurrentLine(int x , int y){
    if(currentLine){
        cout << "updating a new Line Object"<<endl;
        currentLine->updateEndPoints( x , y );
    }
}

/*
 will be called when user presses left mouse button.
 */
void startLineDraw(int x , int y){
    
    cout << "Creating a new Line Object"<<endl;
    currentLine = new Line(x , y , x , y);
}


/*
 Will be called when user releases mouse. this method will add the current line to list of lines and
 cleanup the currentLine pointer.
 */
void commitCurrentLine(){
    
    drawableObjects.push_back(currentLine);
    currentLine = NULL;
    
}

/********************************************************************************************************************************
                                Mouse Control Methods
 *******************************************************************************************************************************/



void mouseClicked(int button , int state , int x , int y){
    /*
     *	update the mouse position
     */
    mouseController.x = x;
    mouseController.y = y;
    
    /*
     *	has the button been pressed or released?
     */
    if (state == GLUT_DOWN)
    {
        
        mouseController.xDragStart = x;
        mouseController.yDragStart = y;
        
        
        switch(button)
        {
            case GLUT_LEFT_BUTTON:
                mouseController.mousePressed = 1;
                
                /*
                 * is current mode is line mode then user click will start a line
                 */
                if(UIMode == UI_MODE_LINE){
                    startLineDraw(x, y);
                }
                break;
            
        }
    }
    else
    {
        /*
         *	Which button was released?
         */
        switch(button)
        {
            case GLUT_LEFT_BUTTON:
                mouseController.mousePressed = 0;
                
                // delete the current curve or line
                if(UIMode == UI_MODE_LINE){
                    commitCurrentLine();
                }

                break;
            
        }
    }
    
    /*
     *	Force a redraw of the screen. If we later want interactions with the mouse
     *	and the 3D scene, we will need to redraw the changes.
     */
    glutPostRedisplay();

}

void mouseMove(int x , int y){
    
    int dx = x - mouseController.x;
    int dy = y - mouseController.y;
    
    mouseController.x = x;
    mouseController.y = y;
    
    switch (UIMode) {
        case UI_MODE_LINE:
            updateCurrentLine(x, y);
            break;
            
        default:
            break;
    }
    
    glutPostRedisplay();
    //cout << "mouse moved by "<< dx << "," << dy;
}





int main (int argc, char** argv)
{
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_RGB|GLUT_DEPTH|GLUT_DOUBLE);
    glutInitWindowSize(WINDOW_WIDTH,WINDOW_HEIGHT);
    glutInitWindowPosition(200,100);
    glutCreateWindow("03 - Mouse Motion");
    
//    DrawableObject *l1 = new Line( 0, 0, 50, 50 );
//    DrawableObject *l2 = new Line( 10, 10, 100, 100 );
//    drawableObjects.push_back(l1);
//    drawableObjects.push_back(l2);
    glutDisplayFunc(draw);
//    glutReshapeFunc(Resize);
      glutMouseFunc(mouseClicked);
      glutMotionFunc(mouseMove);
//    glutPassiveMotionFunc(MousePassiveMotion);
    
    init();
    
    glutMainLoop();
    return 1;
}
