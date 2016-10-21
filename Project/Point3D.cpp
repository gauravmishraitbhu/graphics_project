//
//  Point3D.cpp
//  Project
//
//  Created by Gaurav Mishra on 21/10/16.
//  Copyright © 2016 Gaurav Mishra. All rights reserved.
//

#include "Point3D.hpp"
#include "Point2D.hpp"


Point3D::Point3D(Point2D sketchPoint){
    x = sketchPoint.x;
    y = sketchPoint.y;
    z = 0;
}
