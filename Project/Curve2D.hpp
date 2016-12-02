//
//  Curve2D.hpp
//  Project
//
//  Created by Gaurav Mishra on 24/11/16.
//  Copyright © 2016 Gaurav Mishra. All rights reserved.
//

#ifndef Curve2D_hpp
#define Curve2D_hpp

#include <vector>
#include <stdio.h>
#include "DrawableObject.hpp"
#include "Point2D.hpp"

using namespace std;
class Line;

class Curve2D : public DrawableObject{
private:
    vector<Point2D> _points;
    
    
public:
    
    Curve2D(int x , int y);
    void draw();
    void extendCurve(int x , int y);
    int getObjectType();
    float getLength();
    Point2D getCurveStartPoint();
    Point2D getCurveEndPoint();
    vector<Point2D> getSamplePoints(Line *proxyLine);
};



#endif /* Curve2D_hpp */
