//
//  Line.cpp
//  Project
//
//  Created by Gaurav Mishra on 28/09/16.
//  Copyright © 2016 Gaurav Mishra. All rights reserved.
//

#include "Line.hpp"
#include "DrawableObject.hpp"
#include <GLUT/GLUT.h>
#include <iostream>
using namespace std;



void Line :: draw(){
    
    // first we need to transform our view coordinates recieved from mouse events to world coordinates
    
    GLint viewport[4]; //var to hold the viewport info
    GLdouble modelview[16]; //var to hold the modelview info
    GLdouble projection[16]; //var to hold the projection matrix info
    GLdouble worldStartX, worldStartY , worldStartZ; //variables to hold world x,y,z coordinates
    GLdouble worldEndX, worldEndY , worldEndZ;
    
    glGetDoublev( GL_MODELVIEW_MATRIX, modelview ); //get the modelview info
    glGetDoublev( GL_PROJECTION_MATRIX, projection ); //get the projection matrix info
    glGetIntegerv( GL_VIEWPORT, viewport ); //get the viewport info
    
    cout << "startX and startY-->" << _startX << ", "<< _startY << endl;
    
    GLfloat winStartX = (float)_startX;
    GLfloat winStartY = (float)viewport[3] - (float)_startY;
    GLfloat winZ = 0;
    
    GLfloat winEndX = (float)_endX;
    GLfloat winEndY = (float)viewport[3] - (float)_endY;
    
    
    gluUnProject( winStartX, winStartY, winZ, modelview, projection, viewport, &worldStartX, &worldStartY, &worldStartZ);
    gluUnProject( winEndX, winEndY, winZ, modelview, projection, viewport, &worldEndX, &worldEndY, &worldEndZ);
    
    cout << "worldStX and worldStY-->" << worldStartX << ", "<<worldStartY << endl;
    cout << "worldENdX and worldENdY-->" << worldEndX << ", "<<worldEndY << endl;
    
    glColor3f (1.0, 0.0, 0.0); // Set line segment color to black.
    glBegin (GL_LINES);
    glVertex2i( worldStartX , worldStartY );
    glVertex2i( worldEndX, worldEndY );
    glEnd ( );
}

void Line::updateEndPoints(int newEndX , int newEndY){
    this->_endX = newEndX;
    this->_endY = newEndY;
}
