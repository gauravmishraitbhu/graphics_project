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

Point getPoint(vector<Point> controlPoints , int index){
    if(index < 0){
        return controlPoints[0];
    }
    
    if(index >= controlPoints.size()){
        return controlPoints[controlPoints.size() - 1];
    }
    
    return controlPoints[index];
}

void Curve3D::draw(){
    glPointSize(4);
    glLineWidth(4);
    glColor3f(1, 0, 1);
    vector<Point> controlPoints;
    
    controlPoints.push_back(_warpedEndpoint1);
    for(Point point:warpedPoints){
        controlPoints.push_back(point);
    }
    controlPoints.push_back(_warpedEndpoint2);
    
    // begin drawing our curve
    glBegin(GL_LINE_STRIP);
    
    // now we have our control points
    // we will run loop from -3 to make the curve clamped
    for(int k = -3,j=0; j<=controlPoints.size(); ++j,++k){
        Point p1 = getPoint(controlPoints, k);
        Point p2 = getPoint(controlPoints, k+1);
        Point p3 = getPoint(controlPoints, k+2);
        Point p4 = getPoint(controlPoints, k+3);
        
        // each curve will be linear combination of these 4 points
        // the weights will be decided by the basis functions
        int LOD = 30;
        for(int i = 0 ; i < LOD ; i++){
            //each curve segment will be created by 20 line segments
            // use the parametric time value 0 to 1 for this curve
            // segment.
            float t = (float)i/LOD;
            
            // the t value inverted
            float it = 1.0f-t;
            
            // calculate weights using the blending functions
            float b0 = it*it*it/6.0f;
            float b1 = (3*t*t*t - 6*t*t +4)/6.0f;
            float b2 = (-3*t*t*t +3*t*t + 3*t + 1)/6.0f;
            float b3 =  t*t*t/6.0f;
            
            float x = b0 * p1.x + b1 * p2.x + b2 * p3.x + b3 * p4.x;
            float y = b0 * p1.y + b1 * p2.y + b2 * p3.y + b3 * p4.y;
            float z = b0 * p1.z + b1 * p2.z + b2 * p3.z + b3 * p4.z;
            
            // specify the point
            glVertex3f( x,y,z );
        }
    }
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

vector<Point> Curve3D::get4ControlPoints(){
    vector<Point> result;
    result.push_back(_endPoint1);
    int numPointsInCurve = (int)pointsIn3D.size();
    
    int point2Index = numPointsInCurve/3;
    int point3Index = 2*numPointsInCurve/3;
    
    result.push_back(pointsIn3D[point2Index]);
    result.push_back(pointsIn3D[point3Index]);
    result.push_back(_endPoint2);
    return result;
}
