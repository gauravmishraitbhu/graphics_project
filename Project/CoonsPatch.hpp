//
//  CoonsPatch.hpp
//  Project
//
//  Created by Gaurav Mishra on 02/12/16.
//  Copyright © 2016 Gaurav Mishra. All rights reserved.
//

#ifndef CoonsPatch_hpp
#define CoonsPatch_hpp

#include <stdio.h>
#include<Point.h>
#include<ParametricObject.h>
#include <vector>
#include <math.h>
#include <iostream>
#include "Face.h"
#include <GLUT/GLUT.h>
using namespace std;

class CoonsPatch:public Face{
private:
    ParametricObject * _c0;
    ParametricObject * _c1;
    ParametricObject * _d0;
    ParametricObject * _d1;
    vector<Point> surfacePoints;
    vector<Point> warpedPoints;
    double _red , _green , _blue;
    
public:
    CoonsPatch(ParametricObject *c0 , ParametricObject* c1 , ParametricObject* d0 , ParametricObject* d1){
        _c0 = c0;
        _c1 = c1;
        _d0 = d0;
        _d1 = d1;
        
        _red = double (rand()) / double(RAND_MAX) ;
        _green = double (rand()) / double(RAND_MAX) ;
        _blue = double (rand()) / double(RAND_MAX) ;
    }
    
    void generateSurfacePoints();
    void warp(int minX , int minY , int minZ , double rangeX , double rangeY , double rangeZ);
    void draw();
};
#endif /* CoonsPatch_hpp */
