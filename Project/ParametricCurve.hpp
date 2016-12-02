//
//  ParametricCurve.hpp
//  Project
//
//  Created by Gaurav Mishra on 02/12/16.
//  Copyright © 2016 Gaurav Mishra. All rights reserved.
//

#ifndef ParametricCurve_hpp
#define ParametricCurve_hpp

#include <stdio.h>
#include "Point.h"
#include <math.h>

/**
 beizier curve representation from 4 control points
 */
class ParametricCurve{
    
private:
    Point _point1{0,0,0};
    Point _point2{0,0,0};
    Point _point3{0,0,0};
    Point _point4{0,0,0};
public:
    ParametricCurve(Point p1,Point p2,Point p3,Point p4){
        _point1 = p1;
        _point2 = p2;
        _point3 = p3;
        _point4 = p4;
    }
    
    //given the parameter s find the interpolated Point from the 4 control points
    Point getValue(double u){
        double oneMinusU = 1-u;
        double b0 = pow(oneMinusU,3);
        double b1 = 3 * u * pow(oneMinusU , 2);
        double b2 = 3 * pow(u,2) * oneMinusU;
        double b3 = pow(u,3);
        
        Point result{0,0,0};
        result.x = b0 * _point1.x + b1 * _point2.x + b2 * _point3.x + b3 * _point4.x;
        result.y = b0 * _point1.y + b1 * _point2.y + b2 * _point3.y + b3 * _point4.y;
        result.z = b0 * _point1.z + b1 * _point2.z + b2 * _point3.z + b3 * _point4.z;
        return result;
    }
    
};
#endif /* ParametricCurve_hpp */
