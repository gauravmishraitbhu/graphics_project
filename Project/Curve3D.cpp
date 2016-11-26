//
//  Curve3D.cpp
//  Project
//
//  Created by Gaurav Mishra on 25/11/16.
//  Copyright © 2016 Gaurav Mishra. All rights reserved.
//

#include "Curve3D.hpp"
#include <GLUT/GLUT.h>
#include <math.h>

void Curve3D::warpCurve(int minX , int minY , int minZ , double rangeX , double rangeY , double rangeZ){
    
    warpedPoints.clear();
    for(Point point:pointsIn3D){
        Point warpedPoint{0,0,0};
        
        warpedPoint.x =  ((point.x - minX) / rangeX) * 5;
        warpedPoint.y =  ((point.y - minY) / rangeY) * 5;
        warpedPoint.z =  ((point.z - minZ) / rangeZ) * 5;
        
        warpedPoints.push_back(warpedPoint);
    }
    
    _warpedEndpoint1.x = ((_endPoint1.x - minX) / rangeX) * 5;
    _warpedEndpoint1.y = ((_endPoint1.y - minY) / rangeY) * 5;
    _warpedEndpoint1.z = ((_endPoint1.z - minZ) / rangeZ) * 5;
    
    _warpedEndpoint2.x = ((_endPoint2.x - minX) / rangeX) * 5;
    _warpedEndpoint2.y = ((_endPoint2.y - minY) / rangeY) * 5;
    _warpedEndpoint2.z = ((_endPoint2.z - minZ) / rangeZ) * 5;
}

void Curve3D::draw(){
    glPointSize(4);
    glLineWidth(4);
    glColor3f(1, 0, 1);
    glBegin(GL_LINE_STRIP);
    glVertex3d(_warpedEndpoint1.x, _warpedEndpoint1.y, _warpedEndpoint1.z);
    for(Point point:warpedPoints){
        glVertex3d(point.x, point.y, point.z);
    }
    glVertex3d(_warpedEndpoint2.x, _warpedEndpoint2.y, _warpedEndpoint2.z);
    glEnd();
    glLineWidth(1);
}

double computeDistance(Point p1 , Point p2){
    double distance = pow( p1.x - p2.x , 2) + pow( p1.y - p2.y , 2) + pow( p1.z - p2.z , 2);
    return sqrt(distance);
}

void Curve3D::addEndpoints(Point point1 , Point point2){
    
    double distance1 = computeDistance(point1,pointsIn3D[0]);
    double distance2 = computeDistance(point2, pointsIn3D[0]);
    
    // if point1 is near to first sample point then thats endpoint 1 for this curve
    if(distance1 < distance2){
        _endPoint1 = point1;
        _endPoint2 = point2;
    }else{
        _endPoint1 = point2;
        _endPoint2 = point1;
    }
    
}
