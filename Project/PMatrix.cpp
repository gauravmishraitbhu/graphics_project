//
//  PMatrix.cpp
//  Project
//
//  Created by Gaurav Mishra on 19/10/16.
//  Copyright © 2016 Gaurav Mishra. All rights reserved.
//

#include "PMatrix.hpp"
#include <Eigen/Dense>
#include <vector>

using namespace Eigen;
using namespace std;

void PMatrix::performSVD(){

    Matrix<float , Dynamic , Dynamic > matrix(numRows , numCols);
    
    //fill up the matrix
    for (int i = 0 ; i < numRows ; i++){
        for(int j = 0 ; j < numCols ; j++){
            matrix(i,j) = data[i][j];
        }
    }
    
    JacobiSVD<MatrixXf> svd(matrix, ComputeThinU | ComputeThinV);
    // need to find the columns of v whose corresponding
    // eigen values are lower than threshold
    
    VectorXf singularVals = svd.singularValues();
    
    float highestVal = singularVals(0);
    
    //we will set threshold as 1/100 of highest value
    float threshold = highestVal / 100;
    
    MatrixXf vMatrix = svd.matrixV();
    vector<int> columnsToTake;
    
    for (int i = 0 ; i < singularVals.size() ; i++){
        float currEigenVal = singularVals(i);
        
        if(currEigenVal < threshold){
            columnsToTake.push_back(i);
        }
    }
    
    //now we know which columns of v matrix to take
    // this matrix will have n + k - 1 rows and columnsToTake.size columns
    // n = vertices k = class
    MatrixXf pNull ( singularVals.size() , columnsToTake.size());
    
    for (int i = 0 ; i < columnsToTake.size() ; i++){
        
        // get the index of column in VMatrix
        int column = columnsToTake.at(i);
        
        // run through all the column entries
        for (int j = 0 ; j < singularVals.size() ; j++){
            pNull(j , i) = vMatrix(j , column);
        }
    }
    
    
    cout << "PNull Computed" << endl;
    cout << pNull << endl;
    
    // do a rref on the pNull
    
    
//    cout << "Its singular values are:" << endl << svd.singularValues() << endl;
//    cout << "Its left singular vectors are the columns of the thin U matrix:" << endl << svd.matrixU() << endl;
//    cout << "Its right singular vectors are the columns of the thin V matrix:" << endl << svd.matrixV() << endl;
}
