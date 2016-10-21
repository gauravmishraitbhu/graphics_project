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
#include <math.h>



using namespace std;


#define PI 3.14159265


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
    
//    cout << "startX and startY-->" << _startX << ", "<< _startY << endl;
    
    GLfloat winStartX = (float)_startX;
    GLfloat winStartY = (float)viewport[3] - (float)_startY;
    GLfloat winZ = 0;
    
    GLfloat winEndX = (float)_endX;
    GLfloat winEndY = (float)viewport[3] - (float)_endY;
    
    
    gluUnProject( winStartX, winStartY, winZ, modelview, projection, viewport, &worldStartX, &worldStartY, &worldStartZ);
    gluUnProject( winEndX, winEndY, winZ, modelview, projection, viewport, &worldEndX, &worldEndY, &worldEndZ);
    
//    cout << "worldStX and worldStY-->" << worldStartX << ", "<<worldStartY << endl;
//    cout << "worldENdX and worldENdY-->" << worldEndX << ", "<<worldEndY << endl;
    glLineWidth(2);
    glColor3f (_colorR, _colorG, _colorB); // Set line segment color to black.
    glBegin (GL_LINES);
    glVertex2i( worldStartX , worldStartY );
    glVertex2i( worldEndX, worldEndY );
    glEnd ( );
    
    // make a big dot to signify direction of vector
    glPointSize(4);
    glBegin(GL_POINTS);
    glVertex2i(worldEndX , worldEndY);
    glEnd();
}

void Line::updateEndPoints(int newEndX , int newEndY){
    this->_endX = newEndX;
    this->_endY = newEndY;
}

float Line::getLength(){
    //sqrt   (x2 + y2);
    return sqrt( pow( _startX - _endX , 2 ) + pow( _startY - _endY , 2) );
}

float Line::getAngle(){
    float radians = atan( (float)(_endY - _startY)/(_endX - _startX) );
    float angle = (radians * 180) / PI;
    if(angle > 85){
        return -1 * angle;
    }else{
        return angle;
    }
}

int Line::getObjectType(){
    return OBJECT_TYPE_LINE;
}

void Line::assignParallelClass(int classNum){
    cout << "setting class to= " << classNum << endl;
    _classNum = classNum;
}


void Line::assignColors(float r , float g , float b){
    _colorR = r;
    _colorG = g;
    _colorB = b;
}


void Line::findAndFixVertex(int x , int y){
    // look at vertex1 ie startX and startY
    if( getCartesianDistance(x , y , _startX , _startY) < 10){
        
        //since the vertex1 is too close to given point move it
        // to that given point
        _startX = x;
        _startY = y;
        _isVertex1Corrected = 1;
    }
    
    //now look at vertex2
    if(getCartesianDistance(x, y, _endX, _endY) < 10 ){
        _endX = x;
        _endY = y;
        _isVertex2Corrected = 1;
    }
}


void Line::uniformifyVertices(){
    float angle = getAngle();
    if(angle <= 45 && angle >= -45){
        
        // check if endX is greater than startX
        // if not then swap
        
        if(_endX < _startX){
            int temp = _endX;
            _endX = _startX;
            _startX = temp;
            
            temp = _endY;
            _endY = _startY;
            _startY = temp;
        }
    }else{
        //check if endY is greater than startY
        
        if(_endY < _startY){
            int temp = _endY;
            _endY = _startY;
            _startY = temp;
            
            temp = _endX;
            _endX = _startX;
            _startX = temp;
        }
    }
}

void Line::alignLineDirection(Line* line){
    // calcualte line1 vector
    int vec1X = line->getV2X() - line->getV1X();
    int vec1Y = line->getV2Y() - line->getV1Y();
    
    // calculate current line's vector
    int thisVecX = _endX - _startX;
    int thisVecY = _endY - _startY;
    
    int dotProduct = vec1X*thisVecX + vec1Y*thisVecY;
    
    if(dotProduct < 0){
        // swap points
        int temp = _endY;
        _endY = _startY;
        _startY = temp;
        
        temp = _endX;
        _endX = _startX;
        _startX = temp;
    }
}

int Line::getNeighborVertexId(int id){
    
    if(_vertex1Id == id){
        return _vertex2Id;
    }
    
    if(_vertex2Id == id){
        return _vertex1Id;
    }
    
    //if none of the vertex matched return -1
    return -1;
}


