//
//  QuadFace.cpp
//  Project
//
//  Created by Gaurav Mishra on 04/12/16.
//  Copyright © 2016 Gaurav Mishra. All rights reserved.
//

#include "QuadFace.hpp"
#include <GLUT/GLUT.h>
void QuadFace :: draw(){
    glColor3d(_red, _green, _blue);
    glBegin(GL_QUADS);
    glVertex3d(_warped1.x, _warped1.y, _warped1.z);
    glVertex3d(_warped2.x, _warped2.y, _warped2.z);
    glVertex3d(_warped3.x, _warped3.y, _warped3.z);
    glVertex3d(_warped4.x, _warped4.y, _warped4.z);
    glEnd();
}

void QuadFace::warp(int minX , int minY , int minZ , double rangeX , double rangeY , double rangeZ){
    _warped1.x =  ((_v1.x - minX) / rangeX) * 5;
    _warped1.y =  ((_v1.y - minY) / rangeY) * 5;
    _warped1.z =  ((_v1.z - minZ) / rangeZ) * 5;
    
    _warped2.x =  ((_v2.x - minX) / rangeX) * 5;
    _warped2.y =  ((_v2.y - minY) / rangeY) * 5;
    _warped2.z =  ((_v2.z - minZ) / rangeZ) * 5;
    
    _warped3.x =  ((_v3.x - minX) / rangeX) * 5;
    _warped3.y =  ((_v3.y - minY) / rangeY) * 5;
    _warped3.z =  ((_v3.z - minZ) / rangeZ) * 5;
    
    _warped4.x =  ((_v4.x - minX) / rangeX) * 5;
    _warped4.y =  ((_v4.y - minY) / rangeY) * 5;
    _warped4.z =  ((_v4.z - minZ) / rangeZ) * 5;
}
