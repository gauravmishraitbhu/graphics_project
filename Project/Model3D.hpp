//
//  Model3D.hpp
//  Project
//
//  Created by Gaurav Mishra on 21/10/16.
//  Copyright © 2016 Gaurav Mishra. All rights reserved.
//


// single instance of this class will exist.
// instantiated from 2D lines and 2D vertices
// but the geometry will start using 3D points

#ifndef Model3D_hpp
#define Model3D_hpp

#include <vector>
#include <stdio.h>
#include <Eigen/Dense>
using namespace std;
using namespace Eigen;

class Line;
class Point2D;
class Point3D;

class Model3D{
    
private:
    
    // these are the 2d lines that user drew on sketch
    vector<Line *> sketchLines;
    
    // 2d vertices
    vector<Point2D> sketchVertices;
    
    vector<Point3D> vertices3D;
    
    double getAngleCostForDegree(int degree);
    
public:
    Model3D(vector<Line *> lines , vector<Point2D> vertices);
    
//    // from the given geometry this method will calculate angle complexity
//    // Cost
//    double getAngleCost();
    
    void optimizeOnAngleCost(MatrixXf pNull , int numParallelClasses);
    
    void updateDepthsOfVertices(vector<float> depths);
};


#endif /* Model3D_hpp */
