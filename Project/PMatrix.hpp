//
//  PMatrix.hpp
//  Project
//
//  Created by Gaurav Mishra on 19/10/16.
//  Copyright © 2016 Gaurav Mishra. All rights reserved.
//

// This is a simple wrapper around a 2d array
// single instance will exists in our application
#ifndef PMatrix_hpp
#define PMatrix_hpp

#include <stdio.h>
#include <iostream>
#include <Eigen/Dense>

using namespace std;

class PMatrix{

private:
    float **data;
    int numRows = -1;
    int numCols = -1 ;
    
    
public:
    PMatrix(int rows , int cols){
        data = new float*[rows];
        for (int i = 0 ; i < rows ; i++){
            data[i] = new float[cols];
        }
        
        for (int i = 0 ; i < rows ; i++){
            for (int j =0 ; j < cols ; j++){
                data[i][j] = 0;
            }
        }
        
        numRows = rows;
        numCols = cols;
        
    }
    
    int getNumRows(){
        return numRows;
    }
    
    int getNumCols(){
        return numCols;
    }
    
    float ** getData(){
        return data;
    }
    
    void prettyPrint(){
        cout << "-----------------------------------------------------------------------------"<<endl;
        for( int i = 0 ; i < numRows ; i++ ){
            for( int j = 0 ; j < numCols ; j++ ){
                cout << (int)data[i][j] << "    ||   ";
            }
            cout <<endl;
        }
        cout << "-------------------------------------------------------------------------------"<<endl;
    }
    
    void performSVD();
};

#endif /* PMatrix_hpp */
