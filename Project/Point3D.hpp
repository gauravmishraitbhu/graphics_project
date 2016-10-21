//
//  Point3D.hpp
//  Project
//
//  Created by Gaurav Mishra on 21/10/16.
//  Copyright © 2016 Gaurav Mishra. All rights reserved.
//

#ifndef Point3D_hpp
#define Point3D_hpp

#include <stdio.h>

class Point2D;


class Point3D{
public:
    int x  = 0 ;
    int y  = 0 ;
    int z = 0 ;
    
    Point3D(int _x , int _y , int _z){
        x = _x;
        y = _y;
        z = _z;
    }
    
    Point3D(Point2D sketchPoint);
    
};

#endif /* Point3D_hpp */
