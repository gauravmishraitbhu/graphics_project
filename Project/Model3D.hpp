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
class Point;
class Curve3D;
class CoonsPatch;
class Face;

class Model3D{
    
private:
    
    // these are the 2d lines that user drew on sketch
    vector<Line *> sketchLines;
    
    // 2d vertices
    vector<Point2D> sketchVertices;
    
    //3d vertices with real depths
    vector<Point3D> vertices3D;
    
    //3d vertices with normalized coordinates
    vector<Point> _transformedVertices;
    vector<Curve3D *> _reconstructedCurves;
    
    double getAngleCostForDegree(int degree);
    
    MatrixXf _sVector;
    bool optmizationDone = false;
    Curve3D* reconstructSingleCurve(Line *line);
    CoonsPatch * constructFace(int line1V1 , int line1V2 , int line2V1 , int line2V2 , int line3V1 , int line3V2,
                               int line4V1 , int line4V2);
    vector<Face *> _faces;
    
public:
    Model3D(vector<Line *> lines , vector<Point2D> vertices);
    
    //    // from the given geometry this method will calculate angle complexity
    //    // Cost
    //    double getAngleCost();
    
    void optimizeOnAngleCost(MatrixXf pNull , int numParallelClasses);
    
    void optimizeOnTotalCostGivenSVector(MatrixXf pNull , int numParallelClasses , MatrixXf sVector);
    
    void updateDepthsOfVertices(vector<float> depths);
    
    void optimizeOnTotalCost(MatrixXf pNull , int numParallelClasses);
    
    void draw(bool drawFaces);
    
    void detectFaces();
    
    void reconstructCurves();
    
    void constructFaces(int FIGURE_NUM);
};


#endif /* Model3D_hpp */
