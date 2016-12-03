//
//  ParametricCurve.cpp
//  Project
//
//  Created by Gaurav Mishra on 02/12/16.
//  Copyright © 2016 Gaurav Mishra. All rights reserved.
//

#include "ParametricCurve.hpp"
//given the parameter s find the interpolated Point from the 4 control points

Point ParametricCurve::getValue(double u){
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

int ParametricCurve::getType(){
    return 2;
}
