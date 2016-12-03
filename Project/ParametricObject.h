//
//  ParametricObject.h
//  Project
//
//  Created by Gaurav Mishra on 02/12/16.
//  Copyright © 2016 Gaurav Mishra. All rights reserved.
//

#ifndef ParametricObject_h
#define ParametricObject_h
#include "Point.h"
class ParametricObject{

public:
    int virtual getType() = 0;
    Point virtual getValue(double u) = 0;
};

#endif /* ParametricObject_h */
