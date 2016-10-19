//
//  Utils.hpp
//  Project
//
//  Created by Gaurav Mishra on 17/10/16.
//  Copyright © 2016 Gaurav Mishra. All rights reserved.
//

#ifndef Utils_hpp
#define Utils_hpp

#include <stdio.h>
#include <iostream>
#include <vector>
#include "Line.hpp"

using namespace std;

const int OBJECT_TYPE_LINE = 1;
const int OBJECT_TYPE_CURVE = 2;
const int OBJECT_TYPE_BUTTON = 3;

void assignParallelClass(vector<Line *> lines);
void fixSketch(vector <Line *> lines);
float getCartesianDistance(int x1 , int y1 , int x2 , int y2);

#endif /* Utils_hpp */
