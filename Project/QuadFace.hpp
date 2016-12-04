//
//  QuadFace.hpp
//  Project
//
//  Created by Gaurav Mishra on 04/12/16.
//  Copyright © 2016 Gaurav Mishra. All rights reserved.
//

#ifndef QuadFace_hpp
#define QuadFace_hpp

#include <stdio.h>
#include <CoonsPatch.hpp>
#include "Point.h"
#include "Face.h"
#include <GLUT/GLUT.h>

class QuadFace:public Face{
    Point _v1{0,0,0} , _v2{0,0,0} , _v3{0,0,0} , _v4{0,0,0};
    Point _warped1{0,0,0} , _warped2{0,0,0} , _warped3{0,0,0} , _warped4{0,0,0};
    double _red , _green , _blue;
    
public:
    QuadFace(Point v1 , Point v2 , Point v3 , Point v4){
        _v1 = v1;
        _v2 = v2;
        _v3 = v3;
        _v4 = v4;
        
        _red = double (rand()) / double(RAND_MAX) ;
        _green = double (rand()) / double(RAND_MAX) ;
        _blue = double (rand()) / double(RAND_MAX) ;

    }
    
    void draw();
    void warp(int minX , int minY , int minZ , double rangeX , double rangeY , double rangeZ);
};
#endif /* QuadFace_hpp */
