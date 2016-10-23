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
#include<GLUT/GLUT.h>

using namespace Eigen;

#define PI 3.14159265

class Point{
public:
    float x=0;
    float y = 0;
    float z = 0;
    Point(float _x , float _y , float _z){
        x = abs(_x);
        y = abs(_y);
        z = abs(_z);
    }
};


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

double getLineLength(Point3D fromPoint , Point3D toPoint){
    // sqrt (x^2 + y^2 + z^2)
    
    double xSquare = pow( toPoint.x - fromPoint.x , 2);
    double ySquare = pow( toPoint.y - fromPoint.y , 2);
    double zSquare = pow( toPoint.z - fromPoint.z , 2);
    
    return sqrt(xSquare + ySquare + zSquare);
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

double getDeterminanatFromEigenValuesForLineLengths(VectorXf eigenValues , vector<Line*> sketchLines){
    // we first calculate UPPER THRESHOLD
    
    float avgSquareSum = 0;
    for(Line * line : sketchLines){
        avgSquareSum += pow(line->getLength() , 2);
    }
    
    float UPPER_THRESHOLD = 0.01 * (avgSquareSum/sketchLines.size());
    const float LOWER_THRESHOLD = 0.0001;
    
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

double getDeterminantFromEigenValuesForAngles(VectorXf eigenValues){
    
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

double getLineCostForDegree(vector<Point3D> vertices3D ,vector<Line*> sketchLines ,int degree){
    // Step1 Construct obseration matrix
    MatrixXf observations(vertices3D.size() , degree);
    
    for (int vertexId  = 1 ; vertexId <= vertices3D.size() ; vertexId++){
        // for each vertex find the 3 other neighboring vertices which
        // are connected to  this vertex via line
        vector<int> neighborVerticesId = getNeighborVertices(sketchLines, vertexId);
        
        // now calculate the 3 lengths
        Point3D currentVertex = vertices3D.at(vertexId - 1);
        Point3D neighbor1 = vertices3D.at(neighborVerticesId.at(0) - 1);
        Point3D neighbor2 = vertices3D.at(neighborVerticesId.at(1) - 1);
        Point3D neighbor3 = vertices3D.at(neighborVerticesId.at(2) - 1);
        
        // these will form 3 lines ie current-->neighbor1 , current-->neighbor2 , current-->neighbor3
        // lets represent that direction vector using Point3D
        
        double line1Length = getLineLength(currentVertex, neighbor1);
        double line2Length = getLineLength(currentVertex, neighbor2);
        double line3Length = getLineLength(currentVertex, neighbor3);
        
        vector<float> lineLengths;
        lineLengths.push_back(line1Length);
        lineLengths.push_back(line2Length);
        lineLengths.push_back(line3Length);
        
        
        // sort the angles
        sort(lineLengths.begin() , lineLengths.end());
        
        for(int i = 0 ; i < lineLengths.size() ; i++){
            observations(vertexId-1 , i) = lineLengths.at(i);
        }
        
    } // end of loop
    
    cout << "Lines Data...." << endl;
    cout << observations << endl;
    
    // now we can compiute covariance matrix
    MatrixXf centered = observations.rowwise() - observations.colwise().mean();
    MatrixXf covarianceMatrix = (centered.adjoint() * centered) / double(observations.rows() - 1);
    
    //cout << "covariance matrix calculated-->"<< endl;
    //cout << covarianceMatrix << endl;
    
    // now we will fing the determinant of the covarianceMatrix matrix using
    // its eigne values
    
    VectorXf eigenValues = computeEigenValues(covarianceMatrix);
    double cost = getDeterminanatFromEigenValuesForLineLengths(eigenValues , sketchLines);
    cout << "line cost for current model=="<<cost << endl;
    return cost;
    
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
    double cost = getDeterminantFromEigenValuesForAngles(eigenValues);
    
    cout << "angle cost for current model=="<<cost << endl;
    return cost;
}

double getAngleCost(vector<Point3D> vertices3D ,vector<Line*> sketchLines){
    return getAngleCostForDegree(vertices3D , sketchLines , 3);
}

double getLineCost(vector<Point3D> vertices3D ,vector<Line*> sketchLines){
    return getLineCostForDegree(vertices3D , sketchLines , 3);
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



double getTotalCostForGivenSVector(MatrixXf pNull , MatrixXf sVector , vector<Point3D> vertices3D , vector<Line *> sketchLines){
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
    double angleCost = getAngleCost(updatedVertices, sketchLines);
    double lineCost = getLineCost(updatedVertices , sketchLines);
    return angleCost * lineCost;
}



MatrixXf updateAndReturnNewSvector(MatrixXf currentVector,vector<int> indexesToUpdate ,double stepSize, bool toIncrease){
    MatrixXf result(currentVector.size() , 1);
    
    int random = rand() % 100;
    
    if(random >=50){
        // 50% chance to change just one number
        indexesToUpdate.pop_back();
    }
    
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

void Model3D::optimizeOnTotalCostGivenSVector(MatrixXf pNull , int numParallelClasses , MatrixXf sVector){
    double initialSearchStepSize = findMinimumLineLength(sketchLines);
    double currentSearchStepSize = initialSearchStepSize;
    
    
    
    double previousCost = getTotalCostForGivenSVector(pNull , sVector , vertices3D , sketchLines);
    
    // we keep on seaching till we have currentStepSize 1/100 of initial value
    while(currentSearchStepSize > initialSearchStepSize/100){
        // now we will increase couple of variables in s
        // we pick 2 random variables from s to update
        vector<int> randomIndexes = getNRandoms((int)sVector.size(), 2);
        
        // increase them by step size
        MatrixXf increaseSvector = updateAndReturnNewSvector(sVector, randomIndexes, currentSearchStepSize, true);
        
        // calculate cost for increase s
        double increasedVectorCost = getTotalCostForGivenSVector(pNull , increaseSvector , vertices3D , sketchLines);
        
        
        // now decrease the s vector by step size
        MatrixXf decreasedSVector = updateAndReturnNewSvector(sVector, randomIndexes, currentSearchStepSize, false);
        
        double decreasedVectorCost = getTotalCostForGivenSVector(pNull , decreasedSVector , vertices3D , sketchLines);
        
        //check which configuration recuced the cost
        if(increasedVectorCost < previousCost){
            // take this and move on
            cout << "increased vector taken in total-"<<increasedVectorCost<<endl;
            sVector = increaseSvector;
            previousCost = increasedVectorCost;
        }else if(decreasedVectorCost < previousCost){
            cout << "decreased vector taken in total-"<<decreasedVectorCost<<endl;
            sVector = decreasedSVector;
            previousCost = decreasedVectorCost;
        }else{
            //if none of them decrease the cost then redue the search size
            cout << "none vector taken in total-"<<endl;
            currentSearchStepSize = 0.9 * currentSearchStepSize;
        }

    } //end of while loop
    
    cout << "Finished Optimizing Over Total Cost now we have final configuration"<<endl;
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

void Model3D::optimizeOnTotalCost(MatrixXf pNull , int numParallelClasses){
    double initialSearchStepSize = findMinimumLineLength(sketchLines);
    double currentSearchStepSize = initialSearchStepSize;
    
    int numColumns = (int)pNull.cols();
    
    // our task is to find a s vector such that pNull * s gives us
    // depths of vertices and these vertices minimize the cost of
    // model
    MatrixXf sVector = MatrixXf::Zero(numColumns , 1);
    
    double previousCost = getTotalCostForGivenSVector(pNull , sVector , vertices3D , sketchLines);
    
    // we keep on seaching till we have currentStepSize 1/100 of initial value
    while(currentSearchStepSize > initialSearchStepSize/100){
        // now we will increase couple of variables in s
        // we pick 2 random variables from s to update
        vector<int> randomIndexes = getNRandoms((int)sVector.size(), 2);
        
        // increase them by step size
        MatrixXf increaseSvector = updateAndReturnNewSvector(sVector, randomIndexes, currentSearchStepSize, true);
        
        // calculate cost for increase s
        double increasedVectorCost = getTotalCostForGivenSVector(pNull , increaseSvector , vertices3D , sketchLines);
        
        
        // now decrease the s vector by step size
        MatrixXf decreasedSVector = updateAndReturnNewSvector(sVector, randomIndexes, currentSearchStepSize, false);
        
        double decreasedVectorCost = getTotalCostForGivenSVector(pNull , decreasedSVector , vertices3D , sketchLines);
        
        //check which configuration recuced the cost
        if(increasedVectorCost < previousCost){
            // take this and move on
            cout << "increased vector taken in total-"<<increasedVectorCost<<endl;
            sVector = increaseSvector;
            previousCost = increasedVectorCost;
        }else if(decreasedVectorCost < previousCost){
            cout << "decreased vector taken in total-"<<decreasedVectorCost<<endl;
            sVector = decreasedSVector;
            previousCost = decreasedVectorCost;
        }else{
            //if none of them decrease the cost then redue the search size
            cout << "none vector taken in total-"<<endl;
            currentSearchStepSize = 0.9 * currentSearchStepSize;
        }
        
    } //end of while loop
    
    cout << "Finished Optimizing Over Total Cost now we have final configuration"<<endl;
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
    
    cout << "final vertices---"<<endl;
    
    for(Point3D point : vertices3D){
        cout << point.x << "   " << point.y  << "    " << point.z << endl;
    }
}

void Model3D::optimizeOnAngleCost(MatrixXf pNull , int numParallelClasses){
    double initialSearchStepSize = findMinimumLineLength(sketchLines);
    cout << "Setting initial step size to-" << initialSearchStepSize << endl;
    
    double currentSearchStepSize = initialSearchStepSize;
    
    
    int numColumns = (int)pNull.cols();
    
    // our task is to find a s vector such that pNull * s gives us
    // depths of vertices and these vertices minimize the cost of
    // model
    MatrixXf sVector = MatrixXf::Zero(numColumns , 1);
    
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
    
    
    
    cout << "Finished Optimizing Over Angles now we have plausible reconstruction---"<<endl;
    
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
    
    cout << "Now we will optimize over C*" << endl;
    cout << sVector << endl;
    optimizeOnTotalCostGivenSVector(pNull, numParallelClasses, sVector);
}


void Model3D::draw(){
    
    Point3D firstVertex{0,0,0};
    
    int maxX = -1;
    int maxY = -1;
    int maxZ = -1;
    for (Point3D vertex : vertices3D){
        if(vertex.x > maxX){
            maxX = vertex.x;
        }
        
        if(vertex.y > maxY){
            maxY = vertex.y;
        }
        
        if(abs(vertex.z) > maxZ){
            maxZ = abs(vertex.z);
        }
        
        
    }
    
    vector<Point> tranformed;
    
    for (Point3D vertex : vertices3D){
        tranformed.push_back(Point{ ((float)vertex.x/ maxX)* 5 ,
            ((float)vertex.y / maxY)* 5 ,
            ((float)vertex.z / maxZ) * 5 } );
    }
    
    glLineWidth(3);
    glColor3f(1, 0, 0);
    glBegin(GL_QUADS);
//    for(Point3D vertex : vertices3D){
//        glVertex3f(vertex.x - firstVertex.x, vertex.y - firstVertex.y, vertex.z - firstVertex.z);
//    }
    
    glColor3f(1, 0, 0);
    glVertex3f(tranformed[0].x, tranformed[0].y, 0.2);
    glVertex3f(tranformed[1].x , tranformed[1].y , 0.2 );
    glVertex3f(tranformed[2].x , tranformed[2].y , 0.2 );
    glVertex3f(tranformed[3].x , tranformed[3].y , 0.2 );
    
    glColor3f(0, 1, 0);
    glVertex3f(tranformed[2].x , tranformed[2].y , tranformed[2].z);
    glVertex3f(tranformed[6].x , tranformed[6].y , tranformed[6].z);
    glVertex3f(tranformed[4].x , tranformed[4].y , tranformed[4].z);
    glVertex3f(tranformed[3].x , tranformed[3].y , tranformed[3].z);
    
    glColor3f(0, 0, 1);
    glVertex3f(tranformed[0].x , tranformed[0].y , tranformed[0].z);
    glVertex3f(tranformed[5].x , tranformed[5].y , tranformed[5].z);
    glVertex3f(tranformed[7].x , tranformed[7].y , tranformed[7].z);
    glVertex3f(tranformed[1].x , tranformed[1].y , tranformed[1].z);
    
    glColor3f(1, 0, 1);
    glVertex3f(tranformed[6].x , tranformed[6].y , tranformed[6].z);
    glVertex3f(tranformed[7].x , tranformed[7].y , tranformed[7].z);
    glVertex3f(tranformed[5].x , tranformed[5].y , tranformed[5].z);
    glVertex3f(tranformed[4].x , tranformed[4].y , tranformed[4].z);
    
    glEnd();
    glLineWidth(1);
}
