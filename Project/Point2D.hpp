//
//  Point2D.hpp
//  Project
//
//  Created by Gaurav Mishra on 19/10/16.
//  Copyright © 2016 Gaurav Mishra. All rights reserved.
//

#ifndef Point2D_hpp
#define Point2D_hpp

#include <stdio.h>

class Point2D{
public:
    int x  = 0 ;
    int y  = 0 ;
    
    Point2D(int _x , int _y){
        x = _x;
        y = _y;
    }
};

#endif /* Point2D_hpp */
