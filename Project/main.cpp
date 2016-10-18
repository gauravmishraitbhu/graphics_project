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
#include "Button.hpp"
#include <vector>
#include "Utils.hpp"
using namespace std;



const int WINDOW_HEIGHT = 480;
const int WINDOW_WIDTH = 640;

const int UI_MODE_LINE = 0;
const int UI_MODE_FREEHAND = 1;


vector< DrawableObject* > drawableObjects;
vector <Button * > buttons;

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
    
    for (Button *btn : buttons){
        btn->draw();
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
    
    if(currentLine != NULL){
        
        if(currentLine->getLength() > 5){
            drawableObjects.push_back(currentLine);
        }else{
            cout << "ignoring a line which was less than 5 " << endl;
        }
        
        currentLine = NULL;
    }
    
    
}

/********************************************************************************************************************************
                                Button Related Methods
 *******************************************************************************************************************************/

void detectParallelLinesClass(){
    cout << "detectParallelLinesClass" << endl;
    vector<Line *> lines;
    for (DrawableObject *obj : drawableObjects){
        if(obj->getObjectType() == OBJECT_TYPE_LINE){
            lines.push_back((Line *)obj);
        }
    }
    assignParallelClass(lines);
}

void eraseAll(){
    cout << "erasing all lines" << endl;
    drawableObjects.clear();
    currentLine = NULL;
    glutPostRedisplay();
}

void initButtons(){
    Button *detectParallelClassBtn  = new Button("Detect" , 10,10,80,30);
    buttons.push_back(detectParallelClassBtn);
    detectParallelClassBtn->addClickCallback(detectParallelLinesClass);
    
    Button *removeAllBtn = new Button("Erase All" , 100 , 10 , 80 , 30);
    buttons.push_back(removeAllBtn);
    removeAllBtn->addClickCallback(eraseAll);
    
}

/*
    Check on which button if any mouse btn was pressed.
 */
int checkButtonPress(int x , int y){
    
    int btnClicked = 0;
    for (Button *btn : buttons){
        if(btn->buttonClickTest(x, y)){
            btn->setState(1);
            btnClicked = 1;
        }

    }
    
    return btnClicked;
    
}

void checkButtonRelease(int x,int y)
{
    
    for(Button *btn : buttons){
        /*
         *	If the mouse button was pressed within the button area
         *	as well as being released on the button.....
         */
        
        if(btn->buttonClickTest(mouseController.xDragStart, mouseController.yDragStart)
           && btn->buttonClickTest(x, y)){
            
            btn->clicked();
        }
        
        btn->setState(0);
        
    }
    glutPostRedisplay();
}


void buttonMousePassive(int x,int y)
{
    int needRedraw = 0;
    
    
    for(Button *btn : buttons){
        /*
         *	if the mouse moved over the control
         */
        if( btn->buttonClickTest(x, y)){
            /*
             *	If the cursor has just arrived over the control, set the highlighted flag
             *	and force a redraw. The screen will not be redrawn again until the mouse
             *	is no longer over this control
             */
//            cout<<"*******************"<<endl;
//            cout <<"inside a button"<<endl;
            if( !btn->isHighlighted() ){
                btn->setHighlighted(1);
                needRedraw = 1;
            }
            
        }else{
            
            /*
             *	If the cursor is no longer over the control, then if the control
             *	is highlighted (ie, the mouse has JUST moved off the control) then
             *	we set the highlighting back to false, and force a redraw.
             */
            if( btn->isHighlighted() ){
                btn->setHighlighted(0);
                needRedraw = 1;
            }
        }
    }
    
    if (needRedraw) {
//        cout << "Redraw"<<endl;
        glutPostRedisplay();
    }
    
}


/********************************************************************************************************************************
                                Mouse Control Methods
 *******************************************************************************************************************************/


/*
    Will be called when user presses or releases a button.
 */
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
                
                
                if( !checkButtonPress(x,y) ){
                    /*
                     * is current mode is line mode then user click will start a line
                     */
                    if(UIMode == UI_MODE_LINE){
                        startLineDraw(x, y);
                    }
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
                checkButtonRelease(x,y);
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


/*
 When user drags the mouse with button clicked
 */
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


/*
    When user moves mouse without holding down the button.
 */
void mousePassiveMotion(int x , int y){
    /*
     *	Calculate how much the mouse actually moved
     */
    int dx = x - mouseController.x;
    int dy = y - mouseController.y;
    
    /*
     *	update the mouse position
     */
    mouseController.x = x;
    mouseController.y = y;
    
    /*
     *	Check MyButton to see if we should highlight it cos the mouse is over it
     */
    buttonMousePassive(x,y);
}



int main (int argc, char** argv)
{
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_RGB|GLUT_DEPTH|GLUT_DOUBLE);
    glutInitWindowSize(WINDOW_WIDTH,WINDOW_HEIGHT);
    glutInitWindowPosition(200,100);
    glutCreateWindow("3D Reconstruction");
    initButtons();
    glutDisplayFunc(draw);
//    glutReshapeFunc(Resize);
      glutMouseFunc(mouseClicked);
      glutMotionFunc(mouseMove);
      glutPassiveMotionFunc(mousePassiveMotion);
    
    init();
    
    glutMainLoop();
    return 1;
}
