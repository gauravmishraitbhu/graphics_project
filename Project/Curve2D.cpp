//
//  Curve2D.cpp
//  Project
//
//  Created by Gaurav Mishra on 24/11/16.
//  Copyright © 2016 Gaurav Mishra. All rights reserved.
//

#include "Curve2D.hpp"
#include "Point2D.hpp"
#include "Utils.hpp"
#include <GLUT/GLUT.h>
#include <math.h>
#include "Line.hpp"

Curve2D::Curve2D(int x , int y){
    Point2D start{x , y};
    _points.push_back(start);
}

int Curve2D::getObjectType(){
    return OBJECT_TYPE_CURVE;
}

void Curve2D::draw(){
    // first we need to transform our view coordinates recieved from mouse events to world coordinates
    
    GLint viewport[4]; //var to hold the viewport info
    GLdouble modelview[16]; //var to hold the modelview info
    GLdouble projection[16]; //var to hold the projection matrix info
    GLdouble worldPointX, worldPointY , worldPointZ; //variables to hold world x,y,z coordinates
    
    
    glGetDoublev( GL_MODELVIEW_MATRIX, modelview ); //get the modelview info
    glGetDoublev( GL_PROJECTION_MATRIX, projection ); //get the projection matrix info
    glGetIntegerv( GL_VIEWPORT, viewport ); //get the viewport info
    
    //    cout << "startX and startY-->" << _startX << ", "<< _startY << endl;
    
    glLineWidth(2);
    glColor3f (0, 0, 0); // Set line segment color to black.
    glBegin (GL_LINE_STRIP);
    
    for(Point2D point:_points){
        GLfloat winStartX = (float)point.x;
        GLfloat winStartY = (float)viewport[3] - (float)point.y;
        GLfloat winZ = 0;
        gluUnProject( winStartX, winStartY, winZ, modelview, projection, viewport, &worldPointX, &worldPointY, &worldPointZ);
        glVertex2i( worldPointX , worldPointY );
    }
    
    glEnd ( );


}


float Curve2D::getLength(){
    int startX = _points[0].x;
    int startY = _points[0].y;
    
    int endX = _points[_points.size()-1].x;
    int endY = _points[_points.size()-1].y;
    
    return sqrt( pow( startX - endX , 2 ) + pow( startY - endY , 2) );
}


void Curve2D::extendCurve(int x , int y){
    Point2D newPoint{x , y};
    _points.push_back(newPoint);
}

Point2D Curve2D::getCurveStartPoint(){
    return _points[0];
}

Point2D Curve2D::getCurveEndPoint(){
    return _points[_points.size()-1];
}

double getDistance(Point2D a , Point2D b){
    double distance = pow(a.x - b.x , 2) + pow(a.y - b.y , 2);
    return sqrt(distance);
}

vector<Point2D> Curve2D::getSamplePoints(){
    
    // return all points if less that 10 points
    if(_points.size() < 10){
        return _points;
    }

    vector<Point2D> samples;
    
    // dont include first and last vertex since they will be added using proxy line
    for (int i = 1 ; i < _points.size()-1 ; i=i+10){
        
        samples.push_back(_points[i]);
    }
    
    return samples;
}



