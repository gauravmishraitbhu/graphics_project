//
//  Model3D.cpp
//  Project
//
//  Created by Gaurav Mishra on 21/10/16.
//  Copyright © 2016 Gaurav Mishra. All rights reserved.
//


#include "Model3D.hpp"
#include "Point3D.hpp"
#include "Point2D.hpp"
#include "Line.hpp"
#include <Eigen/Dense>
#include<math.h>
#include <algorithm>

using namespace Eigen;

#define PI 3.14159265

Model3D :: Model3D(vector<Line *> lines , vector<Point2D> vertices){
    sketchLines = lines;
    sketchVertices = vertices;
    
    for(Point2D sketchVertex : vertices){
        Point3D vertex3D(sketchVertex);
        vertices3D.push_back(vertex3D);
    }
}

vector<int> getNeighborVertices(vector<Line*> sketchLines , int targetVertexId){
    vector<int> result;
    for(Line * line : sketchLines){
        int neighborId = line->getNeighborVertexId(targetVertexId);
        if(neighborId != -1){
            result.push_back(neighborId);
        }
    }
    
    if(result.size() != 3 ){
        cout << "ERROR : more than 3 neighbors detected" << endl;
    }
    
    return result;
}

Point3D getDirectionVector(Point3D fromPoint , Point3D toPoint){
    Point3D result(toPoint.x - fromPoint.x , toPoint.y - fromPoint.y , toPoint.z - fromPoint.z);
    return result;
}

float getAngleBetweenVectors(Point3D vector1 , Point3D vector2){
    // we will use the formula for dot product
    // A.B = |A| |B| cos(theta)
    
    float length1 =  pow(vector1.x , 2) + pow(vector1.y , 2) + pow(vector1.z , 2);
    length1 = sqrt(length1);
    
    float length2 =  pow(vector2.x , 2) + pow(vector2.y , 2) + pow(vector2.z , 2);
    length2 = sqrt(length2);
    
    float dotProduct = vector1.x * vector2.x + vector1.y * vector2.y + vector1.z * vector2.z;
    
    float cosTheta = dotProduct / (length1 * length2);
    
    float radians = acos(cosTheta);
    
    float angle = (radians * 180) / PI;
    
    return angle;
}

double Model3D :: getAngleCost(){
    // ideally run for all degrees
    // lets start at t=3
    // we will consider 3 degree vertices , each vertex will be represeted as
    // 3 column vector
    
    // Step1 Construct obseration matrix
    MatrixXf observations(vertices3D.size() , 3);
    
    for (int vertexId  = 1 ; vertexId <= vertices3D.size() ; vertexId++){
        // for each vertex find the 3 other neighboring vertices which
        // are connected to  this vertex via line
        vector<int> neighborVerticesId = getNeighborVertices(sketchLines, vertexId);
        
        // now calculate the 3 angles
        Point3D currentVertex = vertices3D.at(vertexId - 1);
        Point3D neighbor1 = vertices3D.at(neighborVerticesId.at(0) - 1);
        Point3D neighbor2 = vertices3D.at(neighborVerticesId.at(1) - 1);
        Point3D neighbor3 = vertices3D.at(neighborVerticesId.at(2) - 1);
        
        // these will form 3 lines ie current-->neighbor1 , current-->neighbor2 , current-->neighbor3
        // lets represent that direction vector using Point3D
        
        Point3D line1 = getDirectionVector(currentVertex, neighbor1);
        Point3D line2 = getDirectionVector(currentVertex, neighbor2);
        Point3D line3 = getDirectionVector(currentVertex, neighbor3);
        
        vector<float> angles;
        angles.push_back(getAngleBetweenVectors(line1 , line2));
        angles.push_back(getAngleBetweenVectors(line2, line3));
        angles.push_back(getAngleBetweenVectors(line3, line1));
        
        
        // sort the angles
        sort(angles.begin() , angles.end());
        
        for(int i = 0 ; i < angles.size() ; i++){
            observations(vertexId-1 , i) = angles.at(i);
        }
        
    } // end of loop
    
    
    cout << "Angles Data...." << endl;
    cout << observations << endl;
    
    // now we can compiute covariance matrix
    MatrixXf centered = observations.rowwise() - observations.colwise().mean();
    MatrixXf covarianceMatrix = (centered.adjoint() * centered) / double(observations.rows() - 1);
    
    cout << "covariance matrix calculated-->"<< endl;
    cout << covarianceMatrix;
    
    return 0;
}


