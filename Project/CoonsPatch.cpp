//
//  CoonsPatch.cpp
//  Project
//
//  Created by Gaurav Mishra on 02/12/16.
//  Copyright © 2016 Gaurav Mishra. All rights reserved.
//

#include "CoonsPatch.hpp"
#include <GLUT/GLUT.h>
#include <iostream>
#include <math.h>
using namespace std;

Point interpolatePoints(Point p1 , Point p2 , double t){
    Point result{0,0,0};
    result.x = p1.x * (1-t) + p2.x * t;
    result.y = p1.y * (1-t) + p2.y * t;
    result.z = p1.z * (1-t) + p2.z * t;
    return result;
}

double bilinearInterpolate(double c0Start , double c0End , double c1Start , double c1End , double s , double t){
    double result;
    result = c0Start * (1-s) * (1-t);
    result += c0End * s * (1-t) ;
    result += c1Start * (1-s) * t;
    result += c1End * s * t;
    return result;
}

void CoonsPatch::generateSurfacePoints(){
    int LOD = 50;
    
    //s and t are two params c0 and c1 are in s direction
    for (int i = 0 ; i < LOD ; i ++){
        for (int j = 0 ; j < LOD ; j++){
            double s = double(i)/double(LOD);
            double t = double(j) / double(LOD);
            
            // compute Lc
            Point c0S = _c0->getValue(s);
            Point c1S = _c1->getValue(s);
            
            Point Lc = interpolatePoints(c0S , c1S , t);
            
            //computeLd
            Point d0T = _d0->getValue(t);
            Point d1T = _d1->getValue(t);
            
            Point Ld = interpolatePoints(d0T , d1T, s);
            
            //compute bST ie interpolate from all corners
            Point bST{0,0,0};
            Point c0Start = _c0->getValue(0);
            Point c0End = _c0->getValue(1);
            
            Point c1Start = _c1->getValue(0);
            Point c1End = _c1->getValue(1);
            bST.x = bilinearInterpolate(c0Start.x, c0End.x, c1Start.x, c1End.x, s, t);
            bST.y = bilinearInterpolate(c0Start.y, c0End.y, c1Start.y, c1End.y, s, t);
            bST.z = bilinearInterpolate(c0Start.z, c0End.z, c1Start.z, c1End.z, s, t);
            
            Point finalPoint{0,0,0};
            finalPoint.x = Lc.x + Ld.x - bST.x;
            finalPoint.y = Lc.y + Ld.y - bST.y;
            finalPoint.z = Lc.z + Ld.z - bST.z;
            
            surfacePoints.push_back(finalPoint);
//            cout << finalPoint.x << "---" << finalPoint.y << " ---- " << finalPoint.z << "\n";
            
        }
    }
}

void CoonsPatch::warp(int minX , int minY , int minZ , double rangeX , double rangeY , double rangeZ){
    
    warpedPoints.clear();
    for(Point point:surfacePoints){
        Point warpedPoint{0,0,0};
        
        warpedPoint.x =  ((point.x - minX) / rangeX) * 5;
        warpedPoint.y =  ((point.y - minY) / rangeY) * 5;
        warpedPoint.z =  ((point.z - minZ) / rangeZ) * 5;
        
        warpedPoints.push_back(warpedPoint);
    }
}

void CoonsPatch::draw(){
    glPointSize(4);
    glColor4f( _red, _green, _blue , 1);
    glBegin(GL_POINTS);
    for (Point point : warpedPoints){
        glVertex3d(point.x, point.y, point.z);
    }
    glEnd();
    glPointSize(1);
}
