//
//  Point.h
//  Project
//
//  Created by Gaurav Mishra on 25/11/16.
//  Copyright © 2016 Gaurav Mishra. All rights reserved.
//

#ifndef Point_h
#define Point_h

#include "Point3D.hpp"
class Point{
public:
    double x=0;
    double y = 0;
    double z = 0;
    Point(double _x , double _y , double _z){
        x = _x;
        y = _y;
        z = _z;
    }
    
    Point(Point3D point){
        x = point.x;
        y = point.y;
        z = point.z;
    }
};


#endif /* Point_h */
