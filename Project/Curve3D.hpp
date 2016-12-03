//
//  Curve3D.hpp
//  Project
//
//  Created by Gaurav Mishra on 25/11/16.
//  Copyright © 2016 Gaurav Mishra. All rights reserved.
//

#ifndef Curve3D_hpp
#define Curve3D_hpp

#include <stdio.h>
#include <vector>
#include "Point.h"
using namespace std;


class Curve3D{
private:
    vector<Point> pointsIn3D;
    vector<Point> warpedPoints;
    Point _endPoint1{0,0,0};
    Point _endPoint2{0,0,0};
    
    Point _warpedEndpoint1{0,0,0};
    Point _warpedEndpoint2{0,0,0};
    
public:
    Curve3D(vector<Point> points){
        pointsIn3D = points;
    
    }
    
    void warpCurve(int minX , int minY , int minZ , double rangeX , double rangeY , double rangeZ);
    
    void draw();
    
    void addEndpoints(Point point1 , Point point2);
    
    vector<Point> get4ControlPoints();
};

#endif /* Curve3D_hpp */
