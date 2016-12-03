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
#include "ParametricObject.h"


/**
 beizier curve representation from 4 control points
 */
class ParametricCurve:public ParametricObject{
    
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
    
    Point getValue(double u);
    int getType();
        
};
#endif /* ParametricCurve_hpp */
