//
//  ParametricLine.hpp
//  Project
//
//  Created by Gaurav Mishra on 02/12/16.
//  Copyright © 2016 Gaurav Mishra. All rights reserved.
//

#ifndef ParametricLine_hpp
#define ParametricLine_hpp

#include <stdio.h>
#include "Point.h"

/**
 line will be represented as v1 + s*diff where s is a param between 0 and 1
 */
class ParametricLine{
    
private:
    Point _endPoint1{0,0,0};
    Point _diff{0,0,0};
public:
    ParametricLine(Point v1 , Point v2){
        _endPoint1 = v1;
        _diff.x = v2.x - v1.x;
        _diff.y = v2.y - v1.y;
        _diff.z = v2.z - v1.z;
    }
    
    Point getValue(double s){
        Point result{0,0,0};
        result.x = _endPoint1.x + _diff.x * s;
        result.y = _endPoint1.y + _diff.y * s;
        result.z = _endPoint1.z + _diff.z * s;
        
        return result;
    }
};
#endif /* ParametricLine_hpp */
