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
#include "PMatrix.hpp"
#include "Point3D.hpp"
#include "Model3D.hpp"
#include "Curve2D.hpp"
using namespace std;



const int WINDOW_HEIGHT = 480;
const int WINDOW_WIDTH = 640;

const int UI_MODE_LINE = 0;
const int UI_MODE_FREEHAND = 1;
const int UI_3D_RENDER = 2;


struct ViewTransform{
    float xRotation = 0;
    float yRotation = 0;
    float zoom = 0;
    float tx = 0;
    float ty = 0;
} viewTransform;



vector< DrawableObject* > drawableObjects;
vector <Button * > buttons;

// index + 1  is vertex id 
// Point2D contains the location
vector<Point2D> verticesIds;

Line *currentLine;
Curve2D *currentCurve;

PMatrix* matrix;
Model3D * model3d;
Button *_toggleDrawModeBtn;

int numParallelClasses = 0;

int mainWindow , otherWindow;

bool toDraw3Dmodel = false;

// 0 -- line Mode , 1 -- free hand , 2 - 3d model shown
int UIMode = UI_MODE_LINE;

struct Mouse
{
    int x;		/*	the x coordinate of the mouse cursor	*/
    int y;		/*	the y coordinate of the mouse cursor	*/
    int mousePressed ;	/*	is the left button pressed?		*/
    
    
    int xDragStart; /*	stores the x-coord of when the first button press occurred	*/
    int yDragStart; /*	stores the y-coord of when the first button press occurred	*/
};


struct Mouse mouseController{0,0,0,0,0};


struct Camera{
    int eyeX;
    int eyeY;
    int eyeZ;
};

struct Camera cameraController{0,1,5};

/* initialize opengl */
void init()
{
//    glEnable(GL_LIGHT0);
    glClearColor (1.0, 1.0, 1.0, 0.0); // Set display-window color to white.
    glMatrixMode (GL_PROJECTION); // Set projection parameters.
    gluOrtho2D (0.0, 640, 0.0, 480);
    
}

void drawTest3Dmodel();

void draw3Dmodel(){
//    gluOrtho2D (0.0, 640, 0.0, 480 );
    model3d->draw();
//    drawTest3Dmodel();
    
}

void initGLFor3D() {
    glEnable(GL_LIGHT0);
    float vAmbientLightBright[4] = {0.5f, 0.5f, 0.5f, 1.0f};
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, vAmbientLightBright);

    glColorMaterial ( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE );
    glEnable ( GL_COLOR_MATERIAL );
    glClearColor(1.0f, 1.0f, 1.0f, 0.0f); // Set background color to black and opaque
    glClearDepth(1.0f);                   // Set background depth to farthest
    glEnable(GL_DEPTH_TEST);   // Enable depth testing for z-culling
    glDepthFunc(GL_LEQUAL);    // Set the type of depth-test
    glShadeModel(GL_SMOOTH);   // Enable smooth shading
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  // Nice perspective corrections
}

void drawTest3Dmodel(){
    glBegin(GL_QUADS);                // Begin drawing the color cube with 6 quads
    // Top face (y = 1.0f)
    // Define vertices in counter-clockwise (CCW) order with normal pointing out
    glColor3f(0.0f, 1.0f, 0.0f);     // Green
    glVertex3f( 1.0f, 1.0f, -1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);
    glVertex3f(-1.0f, 1.0f,  1.0f);
    glVertex3f( 1.0f, 1.0f,  1.0f);
    
    // Bottom face (y = -1.0f)
    glColor3f(1.0f, 0.5f, 0.0f);     // Orange
    glVertex3f( 1.0f, -1.0f,  1.0f);
    glVertex3f(-1.0f, -1.0f,  1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f( 1.0f, -1.0f, -1.0f);
    
    // Front face  (z = 1.0f)
    glColor3f(1.0f, 0.0f, 0.0f);     // Red
    glVertex3f( 1.0f,  1.0f, 1.0f);
    glVertex3f(-1.0f,  1.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);
    glVertex3f( 1.0f, -1.0f, 1.0f);
    
    // Back face (z = -1.0f)
    glColor3f(1.0f, 1.0f, 0.0f);     // Yellow
    glVertex3f( 1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f,  1.0f, -1.0f);
    glVertex3f( 1.0f,  1.0f, -1.0f);
    
    // Left face (x = -1.0f)
    glColor3f(0.0f, 0.0f, 1.0f);     // Blue
    glVertex3f(-1.0f,  1.0f,  1.0f);
    glVertex3f(-1.0f,  1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f,  1.0f);
    
    // Right face (x = 1.0f)
    glColor3f(1.0f, 0.0f, 1.0f);     // Magenta
    glVertex3f(1.0f,  1.0f, -1.0f);
    glVertex3f(1.0f,  1.0f,  1.0f);
    glVertex3f(1.0f, -1.0f,  1.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);
    glEnd();  // End of drawing color-cube//    glutSolidTeapot(1);
}


/*----------------------------------------------------------------------------------------
 *	draw 3d objects in the scene */
void draw3D()
{
    gluLookAt(cameraController.eyeX,cameraController.eyeY,cameraController.eyeZ,0,0,0,0,1,0);
    glTranslatef(0,0,-viewTransform.zoom);
    glTranslatef(viewTransform.tx,viewTransform.ty,0);
    glRotatef(viewTransform.xRotation,1,0,0);
    glRotatef(viewTransform.yRotation,0,1,0);
    glColor4f(0, 0, 0 ,0.2);
    glLineWidth(0.5);
    glBegin(GL_LINES);
    for(int i=-10;i<=10;++i) {
        
        int length = 20;
        
        if(i == 0){
            length = 0;
        }
        // x z plane
        glVertex3f(2*i,0,-1 * length);
        glVertex3f(2*i,0,length);
        
        glVertex3f(length,0,2*i);
        glVertex3f(-1 * length,0,2*i);
        
        // x y plane
        glVertex3f(length,2*i,0);
        glVertex3f(-1 * length,2*i,0);
        
        glVertex3f(2*i,length,0);
        glVertex3f(2*i,-1 * length,0);
        
    }
    glEnd();
    
    glColor3f(1, 0, 0);
    glLineWidth(2);
    glBegin(GL_LINES);
    
    // positive x axis
    glVertex3f(0,0,0);
    glVertex3f(40,0,0);
    
    // positive y axis
    glVertex3f(0,0,0);
    glVertex3f(0,40,0);
    
    //positive z axis
    glVertex3f(0,0,0);
    glVertex3f(0,0,40);
    glEnd();
    glLineWidth(1);
    
    draw3Dmodel();
    
}

void drawText(void *font,const char *text,int x,int y)
{
    glRasterPos2i(x, y);
    
    while( *text != '\0' )
    {
        glutBitmapCharacter( font, *text );
        ++text;
    }
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
    
    if(currentCurve != NULL){
        currentCurve->draw();
    }
    
    for (Button *btn : buttons){
        btn->draw();
    }
    glColor3f(1,0,0);
    if(verticesIds.size() > 0){
        for (int i = 0 ; i < verticesIds.size() ; i++){
            string label = to_string(i + 1);
            Point2D vertex = verticesIds.at(i);
            drawText(GLUT_BITMAP_TIMES_ROMAN_24 , label.c_str() , vertex.x , vertex.y   );
            
        }
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
    
    if(toDraw3Dmodel){
        /*
         *	Enable lighting and the z-buffer
         */
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_LIGHTING);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        
        /*
         *	Set perspective viewing transformation
         */
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(45,(WINDOW_HEIGHT==0)?(1):((float)WINDOW_WIDTH/WINDOW_HEIGHT),0.1f,1000);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        
        /*
         *	Draw the 3D elements in the scene
         */
        
        draw3D();

    }else{
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

    }
    
    
    
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

void extendCurrentCurve(int x , int y){
    if(currentCurve){
        currentCurve->extendCurve(x,y);
    }
}

void startCurveDraw(int x , int y){
    cout << "Creating a new curve" << endl;
    currentCurve = new Curve2D(x , y);
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

void commitCurrentCurve(){
    
    if(currentCurve != NULL){
        
        if(currentCurve->getLength() > 5){
            drawableObjects.push_back(currentCurve);
        }else{
            cout << "ignoring a curve which was less than 5 " << endl;
        }
        
        currentCurve = NULL;
    }
    
    
}


/********************************************************************************************************************************
                                Button Related Methods
 *******************************************************************************************************************************/
void replaceCurvesWithProxyLines(){
    
    vector<Curve2D *> curves;
    vector<int> indexesToRemove;
    int currIndex=-1;
    vector<DrawableObject*> newList;
    for(DrawableObject *obj:drawableObjects){
        currIndex ++;
        
        //we keep only the lines in new list
        
        if(obj->getObjectType() == OBJECT_TYPE_CURVE){
            curves.push_back((Curve2D*)obj);
            indexesToRemove.push_back(currIndex);
        }else{
            newList.push_back(obj);
        }
    }
    
    //clear the old list and add just the lines
    drawableObjects.clear();
    for (DrawableObject *obj : newList){
        drawableObjects.push_back(obj);
    }
    
    for(Curve2D *curve:curves){
        Point2D startPoint = curve->getCurveStartPoint();
        Point2D endPoint = curve->getCurveEndPoint();
        Line *proxyLine = new Line(startPoint.x , startPoint.y , endPoint.x , endPoint.y , curve);
        drawableObjects.push_back(proxyLine);
    }
}


void detectParallelLinesClass(){
    cout << "detectParallelLinesClass" << endl;
    vector<Line *> lines;
    
    replaceCurvesWithProxyLines();
    
    for (DrawableObject *obj : drawableObjects){
        if(obj->getObjectType() == OBJECT_TYPE_LINE){
            lines.push_back((Line *)obj);
        }
    }
    fixSketch(lines);
    
    // make the edge vectors uniform
    for(Line * line : lines ){
        line->uniformifyVertices();
        cout << line->getV1X() << "    " << line->getV1Y() << endl;
        cout << line->getV2X() << "    " << line->getV2Y() << endl;
    }
    
    numParallelClasses = assignParallelClass(lines);
    glutPostRedisplay();
}

void toggleDrawModes(){
    if(UIMode == UI_MODE_LINE){
        UIMode = UI_MODE_FREEHAND;
        _toggleDrawModeBtn->updateLabel("Draw Line");
    }else{
        UIMode = UI_MODE_LINE;
        _toggleDrawModeBtn->updateLabel("Draw Curve");
    }
    glutPostRedisplay();
}

void eraseAll(){
    cout << "erasing all lines" << endl;
    drawableObjects.clear();
    verticesIds.clear();
    currentLine = NULL;
    currentCurve = NULL;
    glutPostRedisplay();
}

void solveSVD(){
    vector<Line *> lines;
    for (DrawableObject *obj : drawableObjects){
        if(obj->getObjectType() == OBJECT_TYPE_LINE){
            lines.push_back((Line *)obj);
        }
    }
    
    fixLineDirections(lines , numParallelClasses);
    verticesIds = assignVertexIds(lines);
    
    int numRows = (int)lines.size();
    int numCols = (int)verticesIds.size() + numParallelClasses - 1;
    
    matrix = new PMatrix(numRows , numCols);
    createPMatrix(matrix , lines , numParallelClasses);
    matrix->prettyPrint();
    matrix->computePNull();
}


void runOptmimationAlgorithm(){
    cout << "Optimization Time...."<<endl;
    vector<Line *> lines;
    for (DrawableObject *obj : drawableObjects){
        if(obj->getObjectType() == OBJECT_TYPE_LINE){
            lines.push_back((Line *)obj);
        }
    }
    if(model3d == NULL){
        model3d = new Model3D(lines , verticesIds);
    }
    
    
    MatrixXf pNull = matrix->getPNullMatrix();
    model3d->optimizeOnAngleCost(pNull , numParallelClasses);
    model3d->reconstructCurves();
    model3d->constructFaces();
//    model3d->optimizeOnTotalCost(pNull, numParallelClasses);
}

void render3dModel(){
    cout << "render 3d model" << endl;
    toDraw3Dmodel = true;
    UIMode = UI_3D_RENDER;
    initGLFor3D();
    glutPostRedisplay();
}

void initButtons(){
    int x = 10;
    int width = 90;
    int height = 25;
    int y = 10;
    int padding = 12;
    
    
    Button *removeAllBtn = new Button("Erase All" , x,y,width,height);
    buttons.push_back(removeAllBtn);
    removeAllBtn->addClickCallback(eraseAll);
    
    x += width + padding;

    Button *toggleDrawModeBtn = new Button("Draw Curve" , x , y , width , height);
    buttons.push_back(toggleDrawModeBtn);
    toggleDrawModeBtn->addClickCallback(toggleDrawModes);
    _toggleDrawModeBtn = toggleDrawModeBtn;
    x += width + padding;
    
    Button *detectParallelClassBtn  = new Button("Detect" , x,y,width,height);
    buttons.push_back(detectParallelClassBtn);
    detectParallelClassBtn->addClickCallback(detectParallelLinesClass);
    
    x += width + padding;
    
    
    Button *solveSVDBtn = new Button("Compute PNull" , x,y,width,height);
    buttons.push_back(solveSVDBtn);
    solveSVDBtn->addClickCallback(solveSVD);
    
    x += width + padding;
    
    Button *beginOptimizeBtn = new Button("Run Optimization" , x,y,width,height);
    buttons.push_back(beginOptimizeBtn);
    beginOptimizeBtn->addClickCallback(runOptmimationAlgorithm);
    
    x += width + padding;
    
    Button *renderBtn = new Button("Render3D" , x,y,width,height);
    buttons.push_back(renderBtn);
    renderBtn->addClickCallback(render3dModel);
    
    x += width + padding;
    
    
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
                    
                    if(UIMode == UI_MODE_FREEHAND){
                        startCurveDraw(x, y);
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
                
                if(UIMode == UI_MODE_FREEHAND){
                    commitCurrentCurve();
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
        case UI_MODE_FREEHAND:
            extendCurrentCurve(x, y);
            break;
        case UI_3D_RENDER:
            viewTransform.xRotation += (float) 0.5f * dy;
            viewTransform.yRotation += (float) 0.5f * dx;
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

void addStartSketch(){
    drawableObjects.push_back(new Line(147,166,147,320));
    drawableObjects.push_back(new Line(147,320,309,320));
    drawableObjects.push_back(new Line(310,167,309,320));
    //drawableObjects.push_back(new Line(147,166,310,167)); //remove this
    drawableObjects.push_back(new Line(310,167,365,120));
    drawableObjects.push_back(new Line(147,166,209,112));
    //drawableObjects.push_back(new Line(209,112,365,120)); // remove this
    drawableObjects.push_back(new Line(365,120,362,261));
    drawableObjects.push_back(new Line(362,261,309,320));
    drawableObjects.push_back(new Line(201,259,147,320));
    drawableObjects.push_back(new Line(209,112,201,259));
    drawableObjects.push_back(new Line(201,259,362,261));
    
    
    
    // prism
//    drawableObjects.push_back(new Line(162,235,104,346));
//    drawableObjects.push_back(new Line(162,235,217,346));
//    
//    drawableObjects.push_back(new Line(104,346,217,346));
//    //drawableObjects.push_back(new Line(162,235,371,187)); // remove this
//    
//    drawableObjects.push_back(new Line(371,187,323,292));
//    drawableObjects.push_back(new Line(323,292,431,292));
//    drawableObjects.push_back(new Line(371,187,431,292));
//    drawableObjects.push_back(new Line(217,345,431,292));
//    drawableObjects.push_back(new Line(104,346,323,292));
    
}

void reshape(GLsizei width, GLsizei height) {  // GLsizei for non-negative integer
    // Compute aspect ratio of the new window
    if (height == 0) height = 1;                // To prevent divide by 0
    GLfloat aspect = (GLfloat)width / (GLfloat)height;
    
    // Set the viewport to cover the new window
    glViewport(0, 0, width, height);
    
    // Set the aspect ratio of the clipping volume to match the viewport
    glMatrixMode(GL_PROJECTION);  // To operate on the Projection matrix
    glLoadIdentity();             // Reset
    // Enable perspective projection with fovy, aspect, zNear and zFar
    gluPerspective(45.0f, aspect, 0.1f, 100.0f);
}

void keyboardHandler(unsigned char ch, int x , int y){
    float stepSize = 1;
    
    // keyboard strokes only when in 3d model viewing mode
    if(UIMode == UI_3D_RENDER){
        if(ch == 'w'){
            
            viewTransform.ty -= stepSize;
        }else if(ch == 's'){

            viewTransform.ty += stepSize;
        }else if(ch == 'a'){

            viewTransform.tx += stepSize;
        }else if(ch == 'd'){
            viewTransform.tx -= stepSize;
        }else if(ch == 'z'){
            viewTransform.zoom += stepSize;
        }else if(ch == 'x'){
            viewTransform.zoom -= stepSize;
        }
        
    }
    
    glutPostRedisplay();
    
}


int main (int argc, char** argv)
{
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_RGB|GLUT_DEPTH|GLUT_DOUBLE);
    glutInitWindowSize(WINDOW_WIDTH,WINDOW_HEIGHT);
    glutInitWindowPosition(200,100);
    mainWindow = glutCreateWindow("3D Reconstruction");
    initButtons();
    glutDisplayFunc(draw);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouseClicked);
    glutMotionFunc(mouseMove);
    glutPassiveMotionFunc(mousePassiveMotion);
    glutKeyboardFunc(keyboardHandler);
    
    init();
//    initGLFor3D();
    addStartSketch();
    glutMainLoop();
    return 1;
}
