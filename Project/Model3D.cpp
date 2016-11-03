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
    double x=0;
    double y = 0;
    double z = 0;
    Point(double _x , double _y , double _z){
        x = _x;
        y = _y;
        z = _z;
    }
};

class Plane {
public:
    int xCoeff;
    int yCoeff;
    int zCoeff;
    int constant;
    
    Plane(int _x , int _y , int _z , int _c){
        xCoeff = _x;
        yCoeff = _y;
        zCoeff = _z;
        constant = _c;
    }
    
    int getPointPositionRelToPlane(int x , int y , int z){
        int sum = xCoeff * x + yCoeff*y + zCoeff*z + constant;
        
        if( sum < 10 && sum > -10 ){
            return 0;
        }else if(sum > 10){
            return 1;
        }else{
            return -1;
        }
    }
    
    double getDistanceFromPlane(int x , int y , int z){
        double dotProduct = xCoeff * x + yCoeff * y + zCoeff + z + constant;
        double denominator = sqrt( pow(xCoeff , 2) + pow(yCoeff , 2) + pow(zCoeff , 2) );
        return dotProduct / denominator;
    }
};


void drawText(void *font,const char *text,double x,double y,double z)
{
    glRasterPos3d(x, y , z);
    
    while( *text != '\0' )
    {
        glutBitmapCharacter( font, *text );
        ++text;
    }
}


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
    
//    cout << "The eigenvalues of A are:\n" << eigensolver.eigenvalues() << endl;
    
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
    
//    cout << "Lines Data...." << endl;
//    cout << observations << endl;
    
    // now we can compiute covariance matrix
    MatrixXf centered = observations.rowwise() - observations.colwise().mean();
    MatrixXf covarianceMatrix = (centered.adjoint() * centered) / double(observations.rows() - 1);
    
    //cout << "covariance matrix calculated-->"<< endl;
    //cout << covarianceMatrix << endl;
    
    // now we will fing the determinant of the covarianceMatrix matrix using
    // its eigne values
    
    VectorXf eigenValues = computeEigenValues(covarianceMatrix);
    double cost = getDeterminanatFromEigenValuesForLineLengths(eigenValues , sketchLines);
//    cout << "line cost for current model=="<<cost << endl;
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
//    assert(depths.size() == 7);
    for(int i = 0 ; i < depths.size() ; i++){
        Point3D vertex = vertices3D.at(i+1);
        vertex.z = depths.at(i);
        vertices3D[i+1] = vertex;
    }
}

vector<int> getNRandoms(int maxNum , int n){
    // given a maxNum we need to find 'n'  random numbers
//    cout << "Pick random between 0 and " << maxNum << endl;
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
        int temp = arr[maxNum - i - 1];
        arr[maxNum - i] = arr[randomNumber];
        arr[randomNumber] = temp;
    }
    
//    cout << results.at(0) << "----" << results.at(1) <<endl;
    return results;
}

double getAngleCostForGivenSVector(MatrixXf pNull , MatrixXf sVector , vector<Point3D> vertices3D , vector<Line *> sketchLines,
                                   int numParallelClasses){
    // find the depths for given svector
    MatrixXf product = pNull*sVector ;
    
    // now we will update the depths of each vertex
    vector<float> depths;
    for(int  i = numParallelClasses ; i < product.rows() ; i++){
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



double getTotalCostForGivenSVector(MatrixXf pNull , MatrixXf sVector , vector<Point3D> vertices3D , vector<Line *> sketchLines,
                                   int numParallelClasses){
    // find the depths for given svector
    MatrixXf product = pNull*sVector ;
    
    // now we will update the depths of each vertex
    vector<float> depths;
    for(int  i = numParallelClasses ; i < product.rows() ; i++){
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
//    MatrixXf result(currentVector.size() , 1);
    MatrixXf result = currentVector;
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
    
    
    
    double previousCost = getTotalCostForGivenSVector(pNull , sVector , vertices3D , sketchLines,numParallelClasses);
    
    // we keep on seaching till we have currentStepSize 1/100 of initial value
    while(currentSearchStepSize > initialSearchStepSize/100){
        // now we will increase couple of variables in s
        // we pick 2 random variables from s to update
        vector<int> randomIndexes = getNRandoms((int)sVector.size(), 2);
        
        // increase them by step size
        MatrixXf increaseSvector = updateAndReturnNewSvector(sVector, randomIndexes, currentSearchStepSize, true);
        
        // calculate cost for increase s
        double increasedVectorCost = getTotalCostForGivenSVector(pNull , increaseSvector , vertices3D , sketchLines,numParallelClasses);
        
        
        // now decrease the s vector by step size
        MatrixXf decreasedSVector = updateAndReturnNewSvector(sVector, randomIndexes, currentSearchStepSize, false);
        
        double decreasedVectorCost = getTotalCostForGivenSVector(pNull , decreasedSVector , vertices3D , sketchLines,numParallelClasses);
        cout << "Costs----"<<endl;
        cout << increasedVectorCost << endl;
        cout << decreasedVectorCost << endl;
        cout << previousCost << endl;
        
//        cout << "************************************Prev S Vector" << endl;
//        cout << sVector << endl;
//        cout << "*************************************increase S vector" << endl;
//        cout << increaseSvector<<endl;
        
        
        
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
            cout << "none vector taken in total-"<<previousCost<<endl;
            currentSearchStepSize = 0.9 * currentSearchStepSize;
        }

    } //end of while loop
    
    cout << "Finished Optimizing Over Total Cost now we have final configuration"<<endl;
    // now lets calculate final depths and update our model
    MatrixXf product = pNull*sVector ;
    
    // now we will update the depths of each vertex
    vector<float> depths;
    for(int  i = numParallelClasses ; i < product.rows() ; i++){
        // these are depths for v2,v3,..... v1 = 0
        depths.push_back(product(i,0));
        cout << product(i,0) << endl;
    }
    updateDepthsOfVertices(depths);
    _sVector = sVector;
    optmizationDone = true;
    
}

void Model3D::optimizeOnTotalCost(MatrixXf pNull , int numParallelClasses){
    double initialSearchStepSize = findMinimumLineLength(sketchLines);
    double currentSearchStepSize = initialSearchStepSize;
    
    int numColumns = (int)pNull.cols();
    
    // our task is to find a s vector such that pNull * s gives us
    // depths of vertices and these vertices minimize the cost of
    // model
    MatrixXf sVector = MatrixXf::Zero(numColumns , 1);
    
    double previousCost = getTotalCostForGivenSVector(pNull , sVector , vertices3D , sketchLines,numParallelClasses);
    
    // we keep on seaching till we have currentStepSize 1/100 of initial value
    while(currentSearchStepSize > initialSearchStepSize/100){
        // now we will increase couple of variables in s
        // we pick 2 random variables from s to update
        vector<int> randomIndexes = getNRandoms((int)sVector.size(), 2);
        
        // increase them by step size
        MatrixXf increaseSvector = updateAndReturnNewSvector(sVector, randomIndexes, currentSearchStepSize, true);
        
        // calculate cost for increase s
        double increasedVectorCost = getTotalCostForGivenSVector(pNull , increaseSvector , vertices3D , sketchLines,numParallelClasses);
        
        
        // now decrease the s vector by step size
        MatrixXf decreasedSVector = updateAndReturnNewSvector(sVector, randomIndexes, currentSearchStepSize, false);
        
        double decreasedVectorCost = getTotalCostForGivenSVector(pNull , decreasedSVector , vertices3D , sketchLines,numParallelClasses);
        
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
    initialSearchStepSize *= 1000;
    cout << "Setting initial step size to-" << initialSearchStepSize << endl;
    
    double currentSearchStepSize = initialSearchStepSize;
    
    
    int numColumns = (int)pNull.cols();
    
    // our task is to find a s vector such that pNull * s gives us
    // depths of vertices and these vertices minimize the cost of
    // model
    MatrixXf sVector;
    if(optmizationDone){
       sVector = _sVector;
        cout << sVector << endl;
    }else{
        sVector = MatrixXf::Zero(numColumns , 1);
    }
    
    cout << "**********************************************"<<endl;
    cout << sVector << endl;
    
    double previousCost = getAngleCostForGivenSVector(pNull , sVector , vertices3D , sketchLines,numParallelClasses);
    
    // we keep on seaching till we have currentStepSize 1/100 of initial value
    while(currentSearchStepSize > initialSearchStepSize/100){
        
        // now we will increase couple of variables in s
        // we pick 2 random variables from s to update
        vector<int> randomIndexes = getNRandoms((int)sVector.size(), 2);
        
        // increase them by step size
        
        MatrixXf increaseSvector = updateAndReturnNewSvector(sVector, randomIndexes, currentSearchStepSize, true);
        
        
        // calculate cost for increase s
        double increasedVectorCost = getAngleCostForGivenSVector(pNull , increaseSvector , vertices3D , sketchLines,numParallelClasses);
        
        
        // now decrease the s vector by step size
        MatrixXf decreasedSVector = updateAndReturnNewSvector(sVector, randomIndexes, currentSearchStepSize, false);
        
        double decreasedVectorCost = getAngleCostForGivenSVector(pNull , decreasedSVector , vertices3D , sketchLines,numParallelClasses);
        
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
        
        cout << "**********************************************"<<endl;
        cout << sVector << endl;
        
    } //end of while loop
    
    
    
    cout << "Finished Optimizing Over Angles now we have plausible reconstruction---"<<endl;
    
    // now lets calculate final depths and update our model
    MatrixXf product = pNull*sVector ;
    
    // now we will update the depths of each vertex
    vector<float> depths;
    for(int  i = numParallelClasses ; i < product.rows() ; i++){
        // these are depths for v2,v3,..... v1 = 0
        depths.push_back(product(i,0));
//        cout << product(i,0) << endl;
    }
    
    updateDepthsOfVertices(depths);
    
    cout << "Now we will optimize over C*" << endl;
    cout << sVector << endl;
    optimizeOnTotalCostGivenSVector(pNull, numParallelClasses, sVector);
}


void Model3D::draw(){
    
    Point3D firstVertex{0,0,0};
    
    int maxX = INT_MIN;
    int maxY = INT_MIN;
    int maxZ = INT_MIN;
    
    int minX = INT_MAX;
    int minY = INT_MAX;
    int minZ = INT_MAX;
    
    for (Point3D vertex : vertices3D){
        if(vertex.x > maxX){
            maxX = vertex.x;
        }
        
        if(vertex.x < minX){
            minX = vertex.x;
        }
        
        if(vertex.y > maxY){
            maxY = vertex.y;
        }
        
        if(vertex.y < minY){
            minY = vertex.y;
        }
        
        if(vertex.z > maxZ){
            maxZ = vertex.z;
        }
        
        if(vertex.z < minZ){
            minZ = vertex.z;
        }
        
    }
    
    double rangeX = maxX - minX;
    double rangeY = maxY - minY;
    double rangeZ = maxZ - minZ;
    
    double aspect = rangeY / rangeX;
    
    // now we will warp our object between 0 to 5 in x , 0 to 5 , 0 to 5 in z

    
    vector<Point> tranformed;
    
    int index = 0;
    for (Point3D vertex : vertices3D){
        
        double transformedX = ( (vertex.x - minX) / rangeX) * 5;
        double transformedY = ( (vertex.y - minY) / rangeY) * 5;
        double transformedZ = 0;
        if (index == 0){
            transformedZ = ( (vertex.z - minZ) / rangeZ ) * 5;
        }else{
            transformedZ = ( (vertex.z - minZ) / rangeZ ) * 5;
        }
        
        Point transPoint{transformedX , transformedY , transformedZ};
//        Point transPoint{static_cast<double>(vertex.x) , static_cast<double>(vertex.y) , static_cast<double>(vertex.z)};
        tranformed.push_back(transPoint);
        index ++ ;
    }
    
    glLineWidth(3);
    glColor3f(1, 0, 0);
    
    
    for (int i = 0 ; i < tranformed.size() ; i++){
        string label = to_string(i + 1);
        Point vertex = tranformed.at(i);
        drawText(GLUT_BITMAP_TIMES_ROMAN_24 , label.c_str() , vertex.x , vertex.y ,vertex.z  );
        
    }
    
    glColor3f(0, 1, 0);
    glBegin(GL_LINES);
    
    for(Line * line : sketchLines){
        int vertex1Id = line->getVertex1Id();
        int vertex2Id = line->getVertex2Id();
        
        Point vertex1 = tranformed[vertex1Id-1];
        Point vertex2 = tranformed[vertex2Id-1];
        
        glVertex3f( vertex1.x, vertex1.y, vertex1.z);
        glVertex3f( vertex2.x, vertex2.y, vertex2.z);
    }
    
    glEnd();
    
//    glBegin(GL_QUADS);
////    for(Point3D vertex : vertices3D){
////        glVertex3f(vertex.x - firstVertex.x, vertex.y - firstVertex.y, vertex.z - firstVertex.z);
////    }
//    
//    glColor3f(1, 0, 0);
//    glVertex3f(tranformed[0].x, tranformed[0].y, tranformed[0].z);
//    glVertex3f(tranformed[1].x , tranformed[1].y , tranformed[1].z );
//    glVertex3f(tranformed[2].x , tranformed[2].y , tranformed[2].z );
//    glVertex3f(tranformed[3].x , tranformed[3].y , tranformed[3].z );
//    
//    glColor3f(0, 1, 0);
//    glVertex3f(tranformed[2].x , tranformed[2].y , tranformed[2].z);
//    glVertex3f(tranformed[6].x , tranformed[6].y , tranformed[6].z);
//    glVertex3f(tranformed[4].x , tranformed[4].y , tranformed[4].z);
//    glVertex3f(tranformed[3].x , tranformed[3].y , tranformed[3].z);
//    
//    glColor3f(0, 0, 1);
//    glVertex3f(tranformed[0].x , tranformed[0].y , tranformed[0].z);
//    glVertex3f(tranformed[5].x , tranformed[5].y , tranformed[5].z);
//    glVertex3f(tranformed[7].x , tranformed[7].y , tranformed[7].z);
//    glVertex3f(tranformed[1].x , tranformed[1].y , tranformed[1].z);
//    
//    glColor3f(1, 0, 1);
//    glVertex3f(tranformed[6].x , tranformed[6].y , tranformed[6].z);
//    glVertex3f(tranformed[7].x , tranformed[7].y , tranformed[7].z);
//    glVertex3f(tranformed[5].x , tranformed[5].y , tranformed[5].z);
//    glVertex3f(tranformed[4].x , tranformed[4].y , tranformed[4].z);
//    
//    glColor3f(1, 1, 0);
//    glVertex3f(tranformed[0].x , tranformed[0].y , tranformed[0].z);
//    glVertex3f(tranformed[3].x , tranformed[3].y , tranformed[3].z);
//    glVertex3f(tranformed[4].x , tranformed[4].y , tranformed[4].z);
//    glVertex3f(tranformed[5].x , tranformed[5].y , tranformed[5].z);
//    
//    glColor3f(0, 1, 1);
//    glVertex3f(tranformed[1].x , tranformed[1].y , tranformed[1].z);
//    glVertex3f(tranformed[7].x , tranformed[7].y , tranformed[7].z);
//    glVertex3f(tranformed[6].x , tranformed[6].y , tranformed[6].z);
//    glVertex3f(tranformed[2].x , tranformed[2].y , tranformed[2].z);
//    
//    glEnd();
    glLineWidth(1);
}


vector<vector<Line *>> getVertexToLineMapping(vector<Line * > lines , vector<Point3D> vertices){
    vector<vector<Line *>> result;
    
    for (int i = 0 ; i < vertices.size() ; i++){
        int vertexId = i + 1;
        vector<Line *> linesForCurrVertex;
        for (Line *line : lines){
            if(line->getVertex1Id() == vertexId || line->getVertex2Id() == vertexId){
                linesForCurrVertex.push_back(line);
            }
        }
        result.push_back(linesForCurrVertex);
    }
    return result;
}

// given a sketch line figure out the line equation in 3d
Point3D get3DLineVector(Line * sketchLine , vector<Point3D> vertices3D){
    int vertex1Id = sketchLine->getVertex1Id();
    int vertex2Id = sketchLine->getVertex2Id();
    
    Point3D vertex1 = vertices3D[vertex1Id - 1];
    Point3D vertex2 = vertices3D[vertex2Id - 1];
    
    Point3D line{vertex2.x - vertex1.x , vertex2.y - vertex1.y , vertex2.z - vertex1.z};
    return line;
}

Point3D computeCrossProduct(Point3D vector1 , Point3D vector2){
    Point3D crossProduct{0,0,0};
    crossProduct.x = vector1.y * vector2.z - vector1.z * vector2.y;
    crossProduct.y = vector1.z - vector2.x - vector1.x * vector2.z;
    crossProduct.z = vector1.x * vector2.y - vector1.y * vector2.x;
    return crossProduct;
}

Plane getPlane(Point3D normal , Point3D point){
    
    // a(X-x) + b(Y-y) + c(Z-z) = 0;
    // c = -ax -by - cz
    
    int constant = -1 * (normal.x * point.x + normal.y * point.y + normal.z * point.z);
    
    Plane plane{normal.x , normal.y , normal.z , constant};
    return plane;
}

void Model3D::detectFaces(){
    // for 1st vertex
    vector<vector<Line *>> vertexToLineMap = getVertexToLineMapping(sketchLines , vertices3D);
    
    vector<Line*> linesForVertex = vertexToLineMap.at(0);
    
    // pick 1st and 2nd line
    // 1st line is formed by 2 vertices
    Point3D line1 = get3DLineVector(linesForVertex[0] , vertices3D);
    
    // 2nd line
    Point3D line2 = get3DLineVector(linesForVertex[1] , vertices3D);
    
    Point3D normalVector = computeCrossProduct(line1 , line2);
    
    Plane plane = getPlane(normalVector, vertices3D[0]);
    
    vector<double> distances;
    for (Point3D vertex : vertices3D){
        distances.push_back(plane.getDistanceFromPlane(vertex.x, vertex.y, vertex.z));
    }
    
}
