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



VectorXf computeEigenValues(MatrixXf covarianceMatrix){
    SelfAdjointEigenSolver<MatrixXf> eigensolver(covarianceMatrix);
    if (eigensolver.info() != Success) abort();
    
    cout << "The eigenvalues of A are:\n" << eigensolver.eigenvalues() << endl;
    
    VectorXf eigenValues = eigensolver.eigenvalues();
    return eigenValues;
}

double getDeterminantFromEigenValues(VectorXf eigenValues){
    
    const float UPPER_THRESHOLD = 1;
    const float LOWER_THRESHOLD = 0.001;
    
    // first we have to filter eigen values acc to following rules-
    // Case 1 : if we find a eigen value which is above UPPER_THRESHOLD then
    // we ignore values which are below UPPER_THRESHOLD
    // Case 2 : if there is no value above UPPER_THRESHOLD
    // here we take all values above LOWER_THRESHOLD
    
    bool case1 = false;
    
    for(int i = 0 ; i < eigenValues.size() ; i++){
        if(eigenValues(i) >= UPPER_THRESHOLD){
            case1 = true;
        }
    }
    
    double productEigenValues = 1;
    if(case1){
        for(int i = 0 ; i < eigenValues.size() ; i++){
            
            // we take only values above upperThreshold
            if(eigenValues(i) >= UPPER_THRESHOLD){
                productEigenValues *= eigenValues(i);
            }
            
        }
    }else{
        for(int i = 0 ; i < eigenValues.size() ; i++){
            if(eigenValues(i) > LOWER_THRESHOLD){
                productEigenValues *= eigenValues(i);
            }
        }
    }
    
    return productEigenValues;
}

double findMinimumLineLength(vector<Line*> lines){
    double minFound = -1;
    
    for (Line * line : lines){
        double length = line->getLength();
        
        if(minFound < 0 || length < minFound){
            minFound = length;
        }
    }
    
    return minFound;
}

double getAngleCostForDegree(vector<Point3D> vertices3D ,vector<Line*> sketchLines ,int degree){
    // ideally run for all degrees
    // lets start at t=3
    // we will consider 3 degree vertices , each vertex will be represeted as
    // 3 column vector
    
    // Step1 Construct obseration matrix
    MatrixXf observations(vertices3D.size() , degree);
    
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
    
    //cout << "covariance matrix calculated-->"<< endl;
    //cout << covarianceMatrix << endl;
    
    // now we will fing the determinant of the covarianceMatrix matrix using
    // its eigne values
    
    VectorXf eigenValues = computeEigenValues(covarianceMatrix);
    double cost = getDeterminantFromEigenValues(eigenValues);
    
    cout << "angle cost for current model=="<<cost << endl;
    return cost;
}

double getAngleCost(vector<Point3D> vertices3D ,vector<Line*> sketchLines){
    return getAngleCostForDegree(vertices3D , sketchLines , 3);
}

void Model3D::updateDepthsOfVertices(vector<float> depths){
    assert(depths.size() == 7);
    for(int i = 0 ; i < depths.size() ; i++){
        Point3D vertex = vertices3D.at(i+1);
        vertex.z = depths.at(i);
        vertices3D[i+1] = vertex;
    }
}

vector<int> getNRandoms(int maxNum , int n){
    // given a maxNum we need to find 'n'  random numbers
    cout << "Pick random between 0 and " << maxNum << endl;
    vector<int> results;
    int arr[maxNum] ;
    
    for(int i = 0 ; i < maxNum ; i++){
        arr[i] = i;
    }
    
    for (int i = 0 ; i < n ; i++){
        // pick a random number between 0 and maxNum - i
        int randomNumber = rand() % (maxNum-i);
        results.push_back(arr[randomNumber]);
        
        //swap random number with the last number
        int temp = arr[maxNum - i];
        arr[maxNum - i] = arr[randomNumber];
        arr[randomNumber] = temp;
    }
    
    cout << results.at(0) << "----" << results.at(1) <<endl;
    return results;
}

double getAngleCostForGivenSVector(MatrixXf pNull , MatrixXf sVector , vector<Point3D> vertices3D , vector<Line *> sketchLines){
    // find the depths for given svector
    MatrixXf product = pNull*sVector ;
    
    // now we will update the depths of each vertex
    vector<float> depths;
    for(int  i = 3 ; i < product.rows() ; i++){
        // these are depths for v2,v3,..... v1 = 0
        depths.push_back(product(i,0));
    }

    // update the vertex depths
    vector<Point3D> updatedVertices;
    
    //simply copy the first element since its not going to change
    updatedVertices.push_back(vertices3D[0]);
    
    for(int i = 0 ; i < depths.size() ; i++){
        Point3D vertex = vertices3D.at(i+1);
        vertex.z = depths.at(i);
        updatedVertices.push_back(vertex);
    }
    
    double cost = getAngleCost(updatedVertices, sketchLines);
    return cost;
}


MatrixXf updateAndReturnNewSvector(MatrixXf currentVector,vector<int> indexesToUpdate ,double stepSize, bool toIncrease){
    MatrixXf result(currentVector.size() , 1);
    
    for(int index : indexesToUpdate){
        float currentVal = currentVector (index, 0);
        
        if(toIncrease){
            currentVal += stepSize;
        }else{
            currentVal -= stepSize;
        }
        //update the s vector
        result(index , 0) = currentVal;
    }
    
    return result;
}

void Model3D::optimizeOnAngleCost(MatrixXf pNull , int numParallelClasses){
    double initialSearchStepSize = findMinimumLineLength(sketchLines);
    cout << "Setting initial step size to-" << initialSearchStepSize << endl;
    
    double currentSearchStepSize = initialSearchStepSize;
    
    
    int numColumns = (int)pNull.cols();
    
    // our task is to find a s vector such that pNull * s gives us
    // depths of vertices and these vertices minimize the cost of
    // model
    MatrixXf sVector = MatrixXf::Random(numColumns , 1);
    
    double previousCost = getAngleCostForGivenSVector(pNull , sVector , vertices3D , sketchLines);
    
    // we keep on seaching till we have currentStepSize 1/100 of initial value
    while(currentSearchStepSize > initialSearchStepSize/100){
        
        // now we will increase couple of variables in s
        // we pick 2 random variables from s to update
        vector<int> randomIndexes = getNRandoms((int)sVector.size(), 2);
        
        // increase them by step size
        MatrixXf increaseSvector = updateAndReturnNewSvector(sVector, randomIndexes, currentSearchStepSize, true);
        
        // calculate cost for increase s
        double increasedVectorCost = getAngleCostForGivenSVector(pNull , increaseSvector , vertices3D , sketchLines);
        
        
        // now decrease the s vector by step size
        MatrixXf decreasedSVector = updateAndReturnNewSvector(sVector, randomIndexes, currentSearchStepSize, false);
        
        double decreasedVectorCost = getAngleCostForGivenSVector(pNull , decreasedSVector , vertices3D , sketchLines);
        
        //check which configuration recuced the cost
        if(increasedVectorCost < previousCost){
            // take this and move on
            cout << "increased vector taken-"<<increasedVectorCost<<endl;
            sVector = increaseSvector;
            previousCost = increasedVectorCost;
        }else if(decreasedVectorCost < previousCost){
            cout << "decreased vector taken-"<<decreasedVectorCost<<endl;
            sVector = decreasedSVector;
            previousCost = decreasedVectorCost;
        }else{
            //if none of them decrease the cost then redue the search size
            cout << "none vector taken-"<<endl;
            currentSearchStepSize = 0.9 * currentSearchStepSize;
        }
        
    } //end of while loop
    
    
    
    cout << "Finished Optimizing---"<<endl;
    // now lets calculate final depths and update our model
    MatrixXf product = pNull*sVector ;
    
    // now we will update the depths of each vertex
    vector<float> depths;
    for(int  i = 3 ; i < product.rows() ; i++){
        // these are depths for v2,v3,..... v1 = 0
        depths.push_back(product(i,0));
        cout << product(i,0) << endl;
    }
    
    updateDepthsOfVertices(depths);
    
}


