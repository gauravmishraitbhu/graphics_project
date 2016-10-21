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
#include <iostream>
#include <iomanip>

using namespace Eigen;
using namespace std;


// converts a matrix to its rref using jacobian elimination
void RowReduce(MatrixXf *matrix)
{
    const int nrows = (int)matrix->rows(); // number of rows
    const int ncols = (int)matrix->cols(); // number of columns
    
    int lead = 0;
    
    while (lead < nrows) {
        float d, m;
        
        for (int r = 0; r < nrows; r++) { // for each row ...
            /* calculate divisor and multiplier */
            d = (*matrix)(lead,lead);
            m = (*matrix)(r,lead) / (*matrix)(lead,lead);
            
            for (int c = 0; c < ncols; c++) { // for each column ...
                if (r == lead)
                    (*matrix)(r,c) /= d;               // make pivot = 1
                else
                    (*matrix)(r,c) -= (*matrix)(lead,c) * m;  // make other = 0
            }
        }
        
        lead++;
        
    }
}


MatrixXf getReducedPNull(MatrixXf pNull){
    // do a rref on the transpose(pNull)
    MatrixXf transposePNull= pNull.transpose();
    
    // now reduce this matrix
    RowReduce(&transposePNull);
    
//    cout << transposePNull << endl;
    
    // take transpose to get pNullReduced
    MatrixXf pNullReduced = transposePNull.transpose();
    
    return pNullReduced;
}

void PMatrix::computePNull(){

    Matrix<float , Dynamic , Dynamic > matrix(numRows , numCols);
    
    //fill up the matrix
    for (int i = 0 ; i < numRows ; i++){
        for(int j = 0 ; j < numCols ; j++){
            matrix(i,j) = data[i][j];
        }
    }
    // the first step is we go a svd of the pMatrix
    // then we take column vectors from V matrix which
    // correpond to singlular values which are nearing 0
    
    
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
    cout << "getting rref of the pNull matrix" << endl;
    
    // now get pNullReduced
    _pNullReduced = getReducedPNull(pNull);
    cout << _pNullReduced << endl;
    
}

